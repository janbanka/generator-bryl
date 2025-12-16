#include "Stozek.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include <cmath>

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
    std::vector<QVector3D> vertices;
    std::vector<GLushort> indices;

    // Apex vertex
    vertices.push_back(QVector3D(0, height / 2, 0));
    // Base center vertex
    vertices.push_back(QVector3D(0, -height / 2, 0));
    // Base circle vertices
    for (int i = 0; i < sides; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(sides);
        vertices.push_back(QVector3D(cos(angle) * radius, -height / 2, sin(angle) * radius));
    }

    // Base indices
    int baseCenterIndex = 1;
    int baseStartIndex = 2;
    for (int i = 0; i < sides; ++i) {
        indices.push_back(baseCenterIndex);
        indices.push_back(baseStartIndex + i);
        indices.push_back(baseStartIndex + ((i + 1) % sides));
    }

    // Side indices
    int apexIndex = 0;
    for (int i = 0; i < sides; ++i) {
        indices.push_back(apexIndex);
        indices.push_back(baseStartIndex + i);
        indices.push_back(baseStartIndex + ((i + 1) % sides));
    }

    m_indexCount = indices.size();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));

    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(indices.data(), indices.size() * sizeof(GLushort));
}

void Stozek::draw(QOpenGLShaderProgram *program, const QMatrix4x4 &projection, const QMatrix4x4 &view, const QMatrix4x4 &model, const QVector3D &color)
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
