#include <cstddef>
#include "Walec.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include <cmath>
#include "Vertex.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Walec::Walec(float radius, float height, int sides)
    : m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ibo(QOpenGLBuffer::IndexBuffer),
      m_indexCount(0)
{
    initializeOpenGLFunctions();
    initialize(radius, height, sides);
}

Walec::~Walec()
{
    m_vbo.destroy();
    m_ibo.destroy();
}

void Walec::initialize(float radius, float height, int sides)
{
    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;
    
    // Top cap
    QVector3D topNormal(0.0f, 1.0f, 0.0f);
    vertices.push_back({QVector3D(0, height / 2, 0), topNormal}); // Top center vertex (index 0)
    for (int i = 0; i < sides; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(sides);
        vertices.push_back({QVector3D(cos(angle) * radius, height / 2, sin(angle) * radius), topNormal}); // Top circle vertices
    }

    // Bottom cap
    QVector3D bottomNormal(0.0f, -1.0f, 0.0f);
    vertices.push_back({QVector3D(0, -height / 2, 0), bottomNormal}); // Bottom center vertex (index sides + 1)
    for (int i = 0; i < sides; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(sides);
        vertices.push_back({QVector3D(cos(angle) * radius, -height / 2, sin(angle) * radius), bottomNormal}); // Bottom circle vertices
    }

    // Sides
    for (int i = 0; i < sides; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(sides);
        QVector3D normal(cos(angle), 0.0f, sin(angle)); // Normal pointing outwards

        // Top ring vertex (for side)
        vertices.push_back({QVector3D(cos(angle) * radius, height / 2, sin(angle) * radius), normal});
        // Bottom ring vertex (for side)
        vertices.push_back({QVector3D(cos(angle) * radius, -height / 2, sin(angle) * radius), normal});
    }

    // Indices for top cap
    int topCenterIndex = 0;
    int topRingStartIndex = 1;
    for (int i = 0; i < sides; ++i) {
        indices.push_back(topCenterIndex);
        indices.push_back(topRingStartIndex + i);
        indices.push_back(topRingStartIndex + ((i + 1) % sides));
    }

    // Indices for bottom cap
    int bottomCenterIndex = sides + 1;
    int bottomRingStartIndex = sides + 2;
    for (int i = 0; i < sides; ++i) {
        indices.push_back(bottomCenterIndex);
        indices.push_back(bottomRingStartIndex + ((i + 1) % sides));
        indices.push_back(bottomRingStartIndex + i);
    }
    
    // Indices for sides
    int sideVerticesStartIndex = (sides + 1) + (sides + 1); // After all cap vertices
    for (int i = 0; i < sides; ++i) {
        int i0 = sideVerticesStartIndex + (i * 2);
        int i1 = sideVerticesStartIndex + (i * 2 + 1);
        int i2 = sideVerticesStartIndex + (((i + 1) % sides) * 2);
        int i3 = sideVerticesStartIndex + (((i + 1) % sides) * 2 + 1);

        indices.push_back(i0); indices.push_back(i1); indices.push_back(i2);
        indices.push_back(i2); indices.push_back(i1); indices.push_back(i3);
    }
    
    m_indexCount = indices.size();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(indices.data(), indices.size() * sizeof(GLushort));
}

void Walec::draw(QOpenGLShaderProgram *program)
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