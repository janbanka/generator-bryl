#ifndef WALEC_H
#define WALEC_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <vector>

#include "Vertex.h"

class Walec : public Bryla, protected QOpenGLFunctions {
public:
    Walec(float radius, float height, int sides);
    ~Walec();
    
    void draw(QOpenGLShaderProgram *program) override;

private:
    void initialize(float radius, float height, int sides);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    int m_indexCount;
};

#endif // WALEC_H