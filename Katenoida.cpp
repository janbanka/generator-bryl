#include <cstddef>
#include "Katenoida.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Katenoida::Katenoida(float c, float v_range, int u_segments, int v_segments)
    : m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ibo(QOpenGLBuffer::IndexBuffer),
      m_indexCount(0)
{
    initializeOpenGLFunctions();
    initialize(c, v_range, u_segments, v_segments);
}

Katenoida::~Katenoida()
{
    m_vbo.destroy();
    m_ibo.destroy();
}

void Katenoida::initialize(float c, float v_range, int u_segments, int v_segments)
{
    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;

    for (int i = 0; i <= u_segments; ++i) {
        float u = 2.0f * M_PI * float(i) / float(u_segments);

        for (int j = 0; j <= v_segments; ++j) {
            float v = -v_range + (2.0f * v_range * float(j) / float(v_segments));
            
            float cosh_v_c = cosh(v / c);

            float x = c * cos(u) * cosh_v_c;
            float y = c * sin(u) * cosh_v_c;
            float z = v;
            
            QVector3D position(x, y, z);

            float nx = - (1.0f/c) * cos(u) * cosh_v_c;
            float ny = - (1.0f/c) * sin(u) * cosh_v_c;
            float nz = sinh(v / c);

            QVector3D normal(nx, ny, nz);
            normal.normalize();

            vertices.push_back({position, normal});
        }
    }

    for (int i = 0; i < u_segments; ++i) {
        for (int j = 0; j < v_segments; ++j) {
            int first = (i * (v_segments + 1)) + j;
            int second = first + v_segments + 1;

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

void Katenoida::draw(QOpenGLShaderProgram *program)
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
