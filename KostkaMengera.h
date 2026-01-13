#ifndef KOSTKAMENGERA_H
#define KOSTKAMENGERA_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <vector>
#include "Vertex.h"

class KostkaMengera : public Bryla, protected QOpenGLFunctions {
public:
    KostkaMengera(int level);
    ~KostkaMengera();
    
    void draw(QOpenGLShaderProgram *program) override;

private:
    void initialize(int level);
    void subdivide(int level, const QVector3D& center, float size);
    void createCube(const QVector3D& center, float size);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    int m_indexCount;
    
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
};

#endif // KOSTKAMENGERA_H
