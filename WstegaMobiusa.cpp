#include <cstddef>
#include "WstegaMobiusa.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

WstegaMobiusa::WstegaMobiusa(float radius, float width, int u_segments, int v_segments)
    : m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ibo(QOpenGLBuffer::IndexBuffer),
      m_indexCount(0)
{
    initializeOpenGLFunctions();
    initialize(radius, width, u_segments, v_segments);
}

WstegaMobiusa::~WstegaMobiusa()
{
    m_vbo.destroy();
    m_ibo.destroy();
}

void WstegaMobiusa::initialize(float radius, float width, int u_segments, int v_segments)
{
    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;

    for (int i = 0; i <= u_segments; ++i) {
        float u = 2.0f * M_PI * float(i) / float(u_segments);
        float cos_u = cos(u);
        float sin_u = sin(u);
        float cos_half_u = cos(u / 2.0f);
        float sin_half_u = sin(u / 2.0f);

        for (int j = 0; j <= v_segments; ++j) {
            float v = -width / 2.0f + (width * float(j) / float(v_segments));
            
            float x = (radius + v * cos_half_u) * cos_u;
            float y = (radius + v * cos_half_u) * sin_u;
            float z = v * sin_half_u;
            
            QVector3D position(x, y, z);

            // Partial derivatives
            float dx_du = -(radius + v * cos_half_u) * sin_u - (v / 2.0f * sin_half_u) * cos_u;
            float dy_du = (radius + v * cos_half_u) * cos_u - (v / 2.0f * sin_half_u) * sin_u;
            float dz_du = (v / 2.0f * cos_half_u);
            QVector3D pu(dx_du, dy_du, dz_du);

            float dx_dv = cos_half_u * cos_u;
            float dy_dv = cos_half_u * sin_u;
            float dz_dv = sin_half_u;
            QVector3D pv(dx_dv, dy_dv, dz_dv);

            QVector3D normal = QVector3D::crossProduct(pu, pv);
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

void WstegaMobiusa::draw(QOpenGLShaderProgram *program)
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
