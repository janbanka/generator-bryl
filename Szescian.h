#ifndef SZESCIAN_H
#define SZESCIAN_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

#include "Vertex.h"

class Szescian : public Bryla, protected QOpenGLFunctions {
public:
    Szescian(float width, float height, float depth);
    ~Szescian();
    
    void draw(QOpenGLShaderProgram *program) override;

private:
    void initialize(float width, float height, float depth);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
};

#endif // SZESCIAN_H
