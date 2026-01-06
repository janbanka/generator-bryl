#include <cstddef>
#include "Szescian.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include "Vertex.h"

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
    
    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;

    // Front face
    QVector3D normalFront(0.0f, 0.0f, 1.0f);
    vertices.push_back({QVector3D(-s, -s, s), normalFront}); // 0
    vertices.push_back({QVector3D(s, -s, s), normalFront});  // 1
    vertices.push_back({QVector3D(s, s, s), normalFront});   // 2
    vertices.push_back({QVector3D(-s, s, s), normalFront});  // 3
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(2); indices.push_back(3); indices.push_back(0);

    // Back face
    QVector3D normalBack(0.0f, 0.0f, -1.0f);
    vertices.push_back({QVector3D(-s, -s, -s), normalBack}); // 4
    vertices.push_back({QVector3D(s, -s, -s), normalBack});  // 5
    vertices.push_back({QVector3D(s, s, -s), normalBack});   // 6
    vertices.push_back({QVector3D(-s, s, -s), normalBack});  // 7
    indices.push_back(4); indices.push_back(5); indices.push_back(6);
    indices.push_back(6); indices.push_back(7); indices.push_back(4);

    // Left face
    QVector3D normalLeft(-1.0f, 0.0f, 0.0f);
    vertices.push_back({QVector3D(-s, -s, -s), normalLeft}); // 8 (originally 4)
    vertices.push_back({QVector3D(-s, -s, s), normalLeft});  // 9 (originally 0)
    vertices.push_back({QVector3D(-s, s, s), normalLeft});   // 10 (originally 3)
    vertices.push_back({QVector3D(-s, s, -s), normalLeft});  // 11 (originally 7)
    indices.push_back(8); indices.push_back(9); indices.push_back(10);
    indices.push_back(10); indices.push_back(11); indices.push_back(8);

    // Right face
    QVector3D normalRight(1.0f, 0.0f, 0.0f);
    vertices.push_back({QVector3D(s, -s, s), normalRight});  // 12 (originally 1)
    vertices.push_back({QVector3D(s, -s, -s), normalRight}); // 13 (originally 5)
    vertices.push_back({QVector3D(s, s, -s), normalRight});  // 14 (originally 6)
    vertices.push_back({QVector3D(s, s, s), normalRight});   // 15 (originally 2)
    indices.push_back(12); indices.push_back(13); indices.push_back(14);
    indices.push_back(14); indices.push_back(15); indices.push_back(12);

    // Top face
    QVector3D normalTop(0.0f, 1.0f, 0.0f);
    vertices.push_back({QVector3D(-s, s, s), normalTop});    // 16 (originally 3)
    vertices.push_back({QVector3D(s, s, s), normalTop});     // 17 (originally 2)
    vertices.push_back({QVector3D(s, s, -s), normalTop});    // 18 (originally 6)
    vertices.push_back({QVector3D(-s, s, -s), normalTop});   // 19 (originally 7)
    indices.push_back(16); indices.push_back(17); indices.push_back(18);
    indices.push_back(18); indices.push_back(19); indices.push_back(16);

    // Bottom face
    QVector3D normalBottom(0.0f, -1.0f, 0.0f);
    vertices.push_back({QVector3D(-s, -s, -s), normalBottom});// 20 (originally 4)
    vertices.push_back({QVector3D(s, -s, -s), normalBottom}); // 21 (originally 5)
    vertices.push_back({QVector3D(s, -s, s), normalBottom});  // 22 (originally 1)
    vertices.push_back({QVector3D(-s, -s, s), normalBottom}); // 23 (originally 0)
    indices.push_back(20); indices.push_back(21); indices.push_back(22);
    indices.push_back(22); indices.push_back(23); indices.push_back(20);

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(indices.data(), indices.size() * sizeof(GLushort));
}

void Szescian::draw(QOpenGLShaderProgram *program)
{
    m_vbo.bind();
    m_ibo.bind();

    int positionLocation = program->attributeLocation("position");
    program->enableAttributeArray(positionLocation);
    program->setAttributeBuffer(positionLocation, GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));

    int normalLocation = program->attributeLocation("normal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);

    program->disableAttributeArray(positionLocation);
    program->disableAttributeArray(normalLocation);
}
