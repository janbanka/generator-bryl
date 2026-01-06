#include <cstddef>
#include "Stozek.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include <cmath>
#include "Vertex.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Stozek::Stozek(float radius, float height, int sides)
    : m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ibo(QOpenGLBuffer::IndexBuffer),
      m_indexCount(0)
{
    initializeOpenGLFunctions();
    initialize(radius, height, sides);
}

Stozek::~Stozek()
{
    m_vbo.destroy();
    m_ibo.destroy();
}

void Stozek::initialize(float radius, float height, int sides)
{
    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;

    // Apex vertex (top of the cone)
    QVector3D apexPosition(0, height / 2, 0);
    // Calculate normal for the sides of the cone
    // The normal vector for a cone side is perpendicular to the surface.
    // It's tricky because it's not a simple (x,y,z) without relation to the position.
    // A vector from the apex to a point on the base circle: (x, -height/2, z) - (0, height/2, 0) = (x, -height, z)
    // The direction of the normal for a side face can be found by taking a vector along the edge
    // and a vector along the tangent of the base circle, then cross product them.
    // Simpler approach for smooth shading: normal points away from the cone axis and upward along the slope.
    // The y component of the normal can be derived from the slope (height/radius).
    // The xz components of the normal will be proportional to the xz coordinates of the vertex on the base.
    
    // Bottom cap
    QVector3D bottomNormal(0.0f, -1.0f, 0.0f);
    vertices.push_back({QVector3D(0, -height / 2, 0), bottomNormal}); // Base center vertex (index 0)
    for (int i = 0; i < sides; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(sides);
        vertices.push_back({QVector3D(cos(angle) * radius, -height / 2, sin(angle) * radius), bottomNormal}); // Base circle vertices (indices 1 to sides)
    }

    // Side vertices and normals
    // Apex vertex needs to be duplicated for each side triangle if we want faceted shading,
    // but for smooth shading, it can be shared with an averaged normal.
    // Given the current simple shader, I'll calculate approximate normals for each vertex
    // that points outwards from the cone.
    
    // Apex vertex for sides (will have varying normal depending on the side face, but for simplified smooth shading,
    // we can use a single normal pointing upwards for the tip, or just rely on interpolated normals)
    // For now, we will add apex at the very end with calculated normals.

    int baseVerticesOffset = 0; // The base center + base circle vertices

    // Indices for bottom cap
    int baseCenterIndex = baseVerticesOffset;
    int baseRingStartIndex = baseVerticesOffset + 1;
    for (int i = 0; i < sides; ++i) {
        indices.push_back(baseCenterIndex);
        indices.push_back(baseRingStartIndex + ((i + 1) % sides));
        indices.push_back(baseRingStartIndex + i);
    }

    // Side vertices
    float normalY = radius / std::sqrt(radius * radius + height * height); // Cosine of angle between cone axis and normal
    float normalXZ = height / std::sqrt(radius * radius + height * height); // Sine of angle
    for (int i = 0; i < sides; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(sides);
        QVector3D currentPos(cos(angle) * radius, -height / 2, sin(angle) * radius);
        QVector3D normal(cos(angle) * normalXZ, normalY, sin(angle) * normalXZ);
        normal.normalize(); // Ensure it's unit length
        
        // Apex vertex with normal pointing roughly towards this segment
        vertices.push_back({apexPosition, normal}); // (index baseVerticesCount + i*2)
        // Base circle vertex for side
        vertices.push_back({currentPos, normal}); // (index baseVerticesCount + i*2 + 1)
    }

    // Side indices
    int sideVerticesStartIndex = baseRingStartIndex + sides; // After bottom cap vertices
    
    for (int i = 0; i < sides; ++i) {
        int apexIdx1 = sideVerticesStartIndex + (i * 2);
        int baseCornerIdx1 = sideVerticesStartIndex + (i * 2) + 1;
        
        int apexIdx2 = sideVerticesStartIndex + (((i + 1) % sides) * 2);
        int baseCornerIdx2 = sideVerticesStartIndex + (((i + 1) % sides) * 2) + 1;

        indices.push_back(apexIdx1);
        indices.push_back(baseCornerIdx1);
        indices.push_back(baseCornerIdx2);
        
        // If we want two triangles for the side face, use two apex indices
        // indices.push_back(apexIdx1);
        // indices.push_back(baseCornerIdx2);
        // indices.push_back(apexIdx2);
    }
    
    m_indexCount = indices.size();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(indices.data(), indices.size() * sizeof(GLushort));
}

void Stozek::draw(QOpenGLShaderProgram *program)
{
    m_vbo.bind();
    m_ibo.bind();

    int positionLocation = program->attributeLocation("position");
    program->enableAttributeArray(positionLocation);
    program->setAttributeBuffer(positionLocation, GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));

    int normalLocation = program->attributeLocation("normal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, nullptr);

    program->disableAttributeArray(positionLocation);
    program->disableAttributeArray(normalLocation);
}
