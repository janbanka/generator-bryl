#include "Kula.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Kula::Kula(float radius, int rings, int sectors)
    : m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ibo(QOpenGLBuffer::IndexBuffer),
      m_indexCount(0)
{
    initializeOpenGLFunctions();
    initialize(radius, rings, sectors);
}

Kula::~Kula()
{
    m_vbo.destroy();
    m_ibo.destroy();
}

void Kula::initialize(float radius, int rings, int sectors)
{
    std::vector<QVector3D> vertices;
    std::vector<GLushort> indices;
    
    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);

    for(int r = 0; r < rings; r++) {
        for(int s = 0; s < sectors; s++) {
            float const y = sin( -M_PI/2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            vertices.push_back(QVector3D(x * radius, y * radius, z * radius));
        }
    }

    for(int r = 0; r < rings-1; r++) {
        for(int s = 0; s < sectors-1; s++) {
            indices.push_back(r * sectors + s);
            indices.push_back(r * sectors + (s+1));
            indices.push_back((r+1) * sectors + (s+1));

            indices.push_back((r+1) * sectors + (s+1));
            indices.push_back((r+1) * sectors + s);
            indices.push_back(r * sectors + s);
        }
    }

    m_indexCount = indices.size();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));

    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(indices.data(), indices.size() * sizeof(GLushort));
}

void Kula::draw(QOpenGLShaderProgram *program, const QMatrix4x4 &projection, const QMatrix4x4 &view, const QMatrix4x4 &model, const QVector3D &color)
{
    program->setUniformValue("projection", projection);
    program->setUniformValue("view", view);
    program->setUniformValue("model", model);
    program->setUniformValue("color", color);

    m_vbo.bind();
    m_ibo.bind();

    int vertexLocation = program->attributeLocation("vertex");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, nullptr);

    program->disableAttributeArray(vertexLocation);
}
