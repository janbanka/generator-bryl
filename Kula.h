#ifndef KULA_H
#define KULA_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

#include "Vertex.h"

class Kula : public Bryla, protected QOpenGLFunctions {
public:
    Kula(float radius, int rings, int sectors);
    ~Kula();
    
    void draw(QOpenGLShaderProgram *program) override;

private:
    void initialize(float radius, int rings, int sectors);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    int m_indexCount;
};

#endif // KULA_H
