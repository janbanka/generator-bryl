#include <cstddef>
#include "Helikoida.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Helikoida::Helikoida(float radius, float rotations, float alpha, int radius_segments, int theta_segments)
    : m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ibo(QOpenGLBuffer::IndexBuffer),
      m_indexCount(0)
{
    initializeOpenGLFunctions();
    initialize(radius, rotations, alpha, radius_segments, theta_segments);
}

Helikoida::~Helikoida()
{
    m_vbo.destroy();
    m_ibo.destroy();
}

void Helikoida::initialize(float radius, float rotations, float alpha, int radius_segments, int theta_segments)
{
    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;

    for (int i = 0; i <= theta_segments; ++i) {
        float theta = rotations * 2.0f * M_PI * float(i) / float(theta_segments);

        for (int j = 0; j <= radius_segments; ++j) {
            float r = radius * float(j) / float(radius_segments);
            
            float x = r * cos(theta);
            float y = r * sin(theta);
            float z = alpha * theta;
            
            QVector3D position(x, y, z);

            float nx = alpha * sin(theta);
            float ny = -alpha * cos(theta);
            float nz = r;

            QVector3D normal(nx, ny, nz);
            normal.normalize();

            vertices.push_back({position, normal});
        }
    }

    for (int i = 0; i < theta_segments; ++i) {
        for (int j = 0; j < radius_segments; ++j) {
            int first = (i * (radius_segments + 1)) + j;
            int second = first + radius_segments + 1;

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

void Helikoida::draw(QOpenGLShaderProgram *program)
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
