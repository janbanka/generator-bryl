#include "Walec.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include <cmath>

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
    std::vector<QVector3D> vertices;
    std::vector<GLushort> indices;
    
    // Using logic from the old MyGLWidget::createCylinder
    // Top center vertex
    vertices.push_back(QVector3D(0, height / 2, 0));
    // Top circle vertices
    for (int i = 0; i < sides; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(sides);
        vertices.push_back(QVector3D(cos(angle) * radius, height / 2, sin(angle) * radius));
    }
    // Bottom center vertex
    vertices.push_back(QVector3D(0, -height / 2, 0));
    // Bottom circle vertices
    for (int i = 0; i < sides; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(sides);
        vertices.push_back(QVector3D(cos(angle) * radius, -height / 2, sin(angle) * radius));
    }

    // Top cap indices
    for (int i = 1; i <= sides; ++i) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back((i % sides) + 1);
    }

    // Bottom cap indices
    int bottomCenterIndex = sides + 1;
    int bottomStartIndex = bottomCenterIndex + 1;
    for (int i = 0; i < sides; ++i) {
        indices.push_back(bottomCenterIndex);
        indices.push_back(bottomStartIndex + i);
        indices.push_back(bottomStartIndex + ((i + 1) % sides));
    }

    // Side indices
    int topStartIndex = 1;
    for (int i = 0; i < sides; ++i) {
        int i0 = topStartIndex + i;
        int i1 = topStartIndex + (i + 1) % sides;
        int i2 = bottomStartIndex + i;
        int i3 = bottomStartIndex + (i + 1) % sides;

        indices.push_back(i0); indices.push_back(i2); indices.push_back(i1);
        indices.push_back(i1); indices.push_back(i2); indices.push_back(i3);
    }
    
    m_indexCount = indices.size();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));

    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(indices.data(), indices.size() * sizeof(GLushort));
}

void Walec::draw(QOpenGLShaderProgram *program, const QMatrix4x4 &projection, const QMatrix4x4 &view, const QMatrix4x4 &model, const QVector3D &color)
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