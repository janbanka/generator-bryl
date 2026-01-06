#ifndef TORUS_H
#define TORUS_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include "Vertex.h"

class Torus : public Bryla, protected QOpenGLFunctions {
public:
    Torus(float majorRadius, float minorRadius, int majorSegments, int minorSegments);
    ~Torus();
    
    void draw(QOpenGLShaderProgram *program) override;

private:
    void initialize(float majorRadius, float minorRadius, int majorSegments, int minorSegments);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    int m_indexCount;
};

#endif // TORUS_H
