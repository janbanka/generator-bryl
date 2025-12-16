#include "Szescian.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>

Szescian::Szescian(float size)
    : m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ibo(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();
    initialize(size);
}

Szescian::~Szescian()
{
    m_vbo.destroy();
    m_ibo.destroy();
}

void Szescian::initialize(float size)
{
    float s = size / 2.0f;
    std::vector<QVector3D> vertices = {
        QVector3D(-s, -s,  s), QVector3D( s, -s,  s),
        QVector3D( s,  s,  s), QVector3D(-s,  s,  s),
        QVector3D(-s, -s, -s), QVector3D( s, -s, -s),
        QVector3D( s,  s, -s), QVector3D(-s,  s, -s)
    };

    std::vector<GLushort> indices = {
        0, 1, 2, 2, 3, 0, // Front
        4, 5, 6, 6, 7, 4, // Back
        0, 4, 7, 7, 3, 0, // Left
        1, 5, 6, 6, 2, 1, // Right
        3, 2, 6, 6, 7, 3, // Top
        0, 1, 5, 5, 4, 0  // Bottom
    };

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));

    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(indices.data(), indices.size() * sizeof(GLushort));
}

void Szescian::draw(QOpenGLShaderProgram *program, const QMatrix4x4 &projection, const QMatrix4x4 &view, const QMatrix4x4 &model, const QVector3D &color)
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

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);

    program->disableAttributeArray(vertexLocation);
}
