#include <cstddef>
#include "MengerSponge.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>

MengerSponge::MengerSponge(int level)
    : m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ibo(QOpenGLBuffer::IndexBuffer),
      m_indexCount(0)
{
    initializeOpenGLFunctions();
    initialize(level);
}

MengerSponge::~MengerSponge()
{
    m_vbo.destroy();
    m_ibo.destroy();
}

void MengerSponge::initialize(int level)
{
    m_vertices.clear();
    m_indices.clear();
    subdivide(level, QVector3D(0.0f, 0.0f, 0.0f), 1.0f);
    
    m_indexCount = m_indices.size();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_vertices.data(), m_vertices.size() * sizeof(Vertex));

    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(m_indices.data(), m_indices.size() * sizeof(GLushort));
}

void MengerSponge::subdivide(int level, const QVector3D& center, float size)
{
    if (level == 0) {
        createCube(center, size);
        return;
    }

    float newSize = size / 3.0f;
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                int sum = abs(x) + abs(y) + abs(z);
                if (sum > 1) {
                    QVector3D newCenter = center + QVector3D(x * newSize, y * newSize, z * newSize);
                    subdivide(level - 1, newCenter, newSize);
                }
            }
        }
    }
}

void MengerSponge::createCube(const QVector3D& center, float size)
{
    float s = size / 2.0f;
    int baseIndex = m_vertices.size();

    // Front face
    QVector3D normalFront(0.0f, 0.0f, 1.0f);
    m_vertices.push_back({center + QVector3D(-s, -s, s), normalFront});
    m_vertices.push_back({center + QVector3D(s, -s, s), normalFront});
    m_vertices.push_back({center + QVector3D(s, s, s), normalFront});
    m_vertices.push_back({center + QVector3D(-s, s, s), normalFront});
    m_indices.push_back(baseIndex + 0); m_indices.push_back(baseIndex + 1); m_indices.push_back(baseIndex + 2);
    m_indices.push_back(baseIndex + 2); m_indices.push_back(baseIndex + 3); m_indices.push_back(baseIndex + 0);

    // Back face
    QVector3D normalBack(0.0f, 0.0f, -1.0f);
    m_vertices.push_back({center + QVector3D(-s, -s, -s), normalBack});
    m_vertices.push_back({center + QVector3D(s, -s, -s), normalBack});
    m_vertices.push_back({center + QVector3D(s, s, -s), normalBack});
    m_vertices.push_back({center + QVector3D(-s, s, -s), normalBack});
    m_indices.push_back(baseIndex + 4); m_indices.push_back(baseIndex + 5); m_indices.push_back(baseIndex + 6);
    m_indices.push_back(baseIndex + 6); m_indices.push_back(baseIndex + 7); m_indices.push_back(baseIndex + 4);
    
    // Left face
    QVector3D normalLeft(-1.0f, 0.0f, 0.0f);
    m_vertices.push_back({center + QVector3D(-s, -s, -s), normalLeft});
    m_vertices.push_back({center + QVector3D(-s, -s, s), normalLeft});
    m_vertices.push_back({center + QVector3D(-s, s, s), normalLeft});
    m_vertices.push_back({center + QVector3D(-s, s, -s), normalLeft});
    m_indices.push_back(baseIndex + 8); m_indices.push_back(baseIndex + 9); m_indices.push_back(baseIndex + 10);
    m_indices.push_back(baseIndex + 10); m_indices.push_back(baseIndex + 11); m_indices.push_back(baseIndex + 8);
    
    // Right face
    QVector3D normalRight(1.0f, 0.0f, 0.0f);
    m_vertices.push_back({center + QVector3D(s, -s, s), normalRight});
    m_vertices.push_back({center + QVector3D(s, -s, -s), normalRight});
    m_vertices.push_back({center + QVector3D(s, s, -s), normalRight});
    m_vertices.push_back({center + QVector3D(s, s, s), normalRight});
    m_indices.push_back(baseIndex + 12); m_indices.push_back(baseIndex + 13); m_indices.push_back(baseIndex + 14);
    m_indices.push_back(baseIndex + 14); m_indices.push_back(baseIndex + 15); m_indices.push_back(baseIndex + 12);

    // Top face
    QVector3D normalTop(0.0f, 1.0f, 0.0f);
    m_vertices.push_back({center + QVector3D(-s, s, s), normalTop});
    m_vertices.push_back({center + QVector3D(s, s, s), normalTop});
    m_vertices.push_back({center + QVector3D(s, s, -s), normalTop});
    m_vertices.push_back({center + QVector3D(-s, s, -s), normalTop});
    m_indices.push_back(baseIndex + 16); m_indices.push_back(baseIndex + 17); m_indices.push_back(baseIndex + 18);
    m_indices.push_back(baseIndex + 18); m_indices.push_back(baseIndex + 19); m_indices.push_back(baseIndex + 16);

    // Bottom face
    QVector3D normalBottom(0.0f, -1.0f, 0.0f);
    m_vertices.push_back({center + QVector3D(-s, -s, -s), normalBottom});
    m_vertices.push_back({center + QVector3D(s, -s, -s), normalBottom});
    m_vertices.push_back({center + QVector3D(s, -s, s), normalBottom});
    m_vertices.push_back({center + QVector3D(-s, -s, s), normalBottom});
    m_indices.push_back(baseIndex + 20); m_indices.push_back(baseIndex + 21); m_indices.push_back(baseIndex + 22);
    m_indices.push_back(baseIndex + 22); m_indices.push_back(baseIndex + 23); m_indices.push_back(baseIndex + 20);
}

void MengerSponge::draw(QOpenGLShaderProgram *program)
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
