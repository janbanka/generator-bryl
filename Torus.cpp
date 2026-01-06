#include <cstddef>
#include "Torus.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Torus::Torus(float majorRadius, float minorRadius, int majorSegments, int minorSegments)
    : m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ibo(QOpenGLBuffer::IndexBuffer),
      m_indexCount(0)
{
    initializeOpenGLFunctions();
    initialize(majorRadius, minorRadius, majorSegments, minorSegments);
}

Torus::~Torus()
{
    m_vbo.destroy();
    m_ibo.destroy();
}

void Torus::initialize(float majorRadius, float minorRadius, int majorSegments, int minorSegments)
{
    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;

    for (int i = 0; i <= majorSegments; ++i) {
        float u = 2.0f * M_PI * float(i) / float(majorSegments);

        for (int j = 0; j <= minorSegments; ++j) {
            float v = 2.0f * M_PI * float(j) / float(minorSegments);

            float x = (majorRadius + minorRadius * cos(v)) * cos(u);
            float y = minorRadius * sin(v);
            float z = (majorRadius + minorRadius * cos(v)) * sin(u);
            
            QVector3D position(x, y, z);

            float nx = cos(v) * cos(u);
            float ny = sin(v);
            float nz = cos(v) * sin(u);

            QVector3D normal(nx, ny, nz);
            normal.normalize();

            vertices.push_back({position, normal});
        }
    }

    for (int i = 0; i < majorSegments; ++i) {
        for (int j = 0; j < minorSegments; ++j) {
            int first = (i * (minorSegments + 1)) + j;
            int second = first + minorSegments + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
    
    m_indexCount = indices.size();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(indices.data(), indices.size() * sizeof(GLushort));
}

void Torus::draw(QOpenGLShaderProgram *program)
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
