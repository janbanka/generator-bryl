#ifndef MENGERSPONGE_H
#define MENGERSPONGE_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <vector>
#include "Vertex.h"

class MengerSponge : public Bryla, protected QOpenGLFunctions {
public:
    MengerSponge(int level);
    ~MengerSponge();
    
    void draw(QOpenGLShaderProgram *program) override;

private:
    void initialize(int level);
    void subdivide(int level, const QVector3D& center, float size);
    void createCube(const QVector3D& center, float size);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    int m_indexCount;
    
    std::vector<Vertex> m_vertices;
    std::vector<GLushort> m_indices;
};

#endif // MENGERSPONGE_H
