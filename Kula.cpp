#include <cstddef>
#include "Kula.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include <cmath>
#include "Vertex.h"
#include <QDebug>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Kula::Kula(float radius, int rings, int sectors)
    : m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ibo(QOpenGLBuffer::IndexBuffer),
      m_indexCount(0)
{
    qDebug() << "Kula constructor: start";
    initializeOpenGLFunctions();
    initialize(radius, rings, sectors);
    qDebug() << "Kula constructor: end";
}

Kula::~Kula()
{
    m_vbo.destroy();
    m_ibo.destroy();
}

void Kula::initialize(float radius, int rings, int sectors)
{
    qDebug() << "Kula initialize: start. radius=" << radius << " rings=" << rings << " sectors=" << sectors;
    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;
    
    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);

    for(int r = 0; r < rings; r++) {
        for(int s = 0; s < sectors; s++) {
            float const y = sin( -M_PI/2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            QVector3D position(x * radius, y * radius, z * radius);
            QVector3D normal = position.normalized(); // Normal is just normalized position for a sphere

            vertices.push_back({position, normal});
        }
    }
    qDebug() << "Kula initialize: vertices generated. count=" << vertices.size();

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
    qDebug() << "Kula initialize: indices generated. count=" << indices.size();

    m_indexCount = indices.size();

    qDebug() << "Kula initialize: creating VBO";
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

    qDebug() << "Kula initialize: creating IBO";
    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(indices.data(), indices.size() * sizeof(GLushort));
    qDebug() << "Kula initialize: end";
}

void Kula::draw(QOpenGLShaderProgram *program)
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
