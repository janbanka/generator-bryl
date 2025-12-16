#ifndef KULA_H
#define KULA_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

class Kula : public Bryla, protected QOpenGLFunctions {
public:
    Kula(float radius, int rings, int sectors);
    ~Kula();
    
    void draw(QOpenGLShaderProgram *program, const QMatrix4x4 &projection, const QMatrix4x4 &view, const QMatrix4x4 &model, const QVector3D &color) override;

private:
    void initialize(float radius, int rings, int sectors);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    int m_indexCount;
};

#endif // KULA_H
