#ifndef STOZEK_H
#define STOZEK_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

#include "Vertex.h"

class Stozek : public Bryla, protected QOpenGLFunctions {
public:
    Stozek(float radius, float height, int sides);
    ~Stozek();
    
    void draw(QOpenGLShaderProgram *program) override;

private:
    void initialize(float radius, float height, int sides);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    int m_indexCount;
};

#endif // STOZEK_H
