#include <cstddef>
#include "Szescian.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include "Vertex.h"

Szescian::Szescian(float width, float height, float depth)
    : m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ibo(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();
    initialize(width, height, depth);
}

Szescian::~Szescian()
{
    m_vbo.destroy();
    m_ibo.destroy();
}

void Szescian::initialize(float width, float height, float depth)
{
    float w = width / 2.0f;
    float h = height / 2.0f;
    float d = depth / 2.0f;
    
    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;

    // Front face
    QVector3D normalFront(0.0f, 0.0f, 1.0f);
    vertices.push_back({QVector3D(-w, -h, d), normalFront}); // 0
    vertices.push_back({QVector3D(w, -h, d), normalFront});  // 1
    vertices.push_back({QVector3D(w, h, d), normalFront});   // 2
    vertices.push_back({QVector3D(-w, h, d), normalFront});  // 3
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(2); indices.push_back(3); indices.push_back(0);

    // Back face
    QVector3D normalBack(0.0f, 0.0f, -1.0f);
    vertices.push_back({QVector3D(-w, -h, -d), normalBack}); // 4
    vertices.push_back({QVector3D(w, -h, -d), normalBack});  // 5
    vertices.push_back({QVector3D(w, h, -d), normalBack});   // 6
    vertices.push_back({QVector3D(-w, h, -d), normalBack});  // 7
    indices.push_back(4); indices.push_back(5); indices.push_back(6);
    indices.push_back(6); indices.push_back(7); indices.push_back(4);

    // Left face
    QVector3D normalLeft(-1.0f, 0.0f, 0.0f);
    vertices.push_back({QVector3D(-w, -h, -d), normalLeft}); // 8
    vertices.push_back({QVector3D(-w, -h, d), normalLeft});  // 9
    vertices.push_back({QVector3D(-w, h, d), normalLeft});   // 10
    vertices.push_back({QVector3D(-w, h, -d), normalLeft});  // 11
    indices.push_back(8); indices.push_back(9); indices.push_back(10);
    indices.push_back(10); indices.push_back(11); indices.push_back(8);

    // Right face
    QVector3D normalRight(1.0f, 0.0f, 0.0f);
    vertices.push_back({QVector3D(w, -h, d), normalRight});  // 12
    vertices.push_back({QVector3D(w, -h, -d), normalRight}); // 13
    vertices.push_back({QVector3D(w, h, -d), normalRight});  // 14
    vertices.push_back({QVector3D(w, h, d), normalRight});   // 15
    indices.push_back(12); indices.push_back(13); indices.push_back(14);
    indices.push_back(14); indices.push_back(15); indices.push_back(12);

    // Top face
    QVector3D normalTop(0.0f, 1.0f, 0.0f);
    vertices.push_back({QVector3D(-w, h, d), normalTop});    // 16
    vertices.push_back({QVector3D(w, h, d), normalTop});     // 17
    vertices.push_back({QVector3D(w, h, -d), normalTop});    // 18
    vertices.push_back({QVector3D(-w, h, -d), normalTop});   // 19
    indices.push_back(16); indices.push_back(17); indices.push_back(18);
    indices.push_back(18); indices.push_back(19); indices.push_back(16);

    // Bottom face
    QVector3D normalBottom(0.0f, -1.0f, 0.0f);
    vertices.push_back({QVector3D(-w, -h, -d), normalBottom});// 20
    vertices.push_back({QVector3D(w, -h, -d), normalBottom}); // 21
    vertices.push_back({QVector3D(w, -h, d), normalBottom});  // 22
    vertices.push_back({QVector3D(-w, -h, d), normalBottom}); // 23
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
