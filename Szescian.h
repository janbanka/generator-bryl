#ifndef SZESCIAN_H
#define SZESCIAN_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

class Szescian : public Bryla, protected QOpenGLFunctions {
public:
    Szescian(float size);
    ~Szescian();
    
    void draw(QOpenGLShaderProgram *program, const QMatrix4x4 &projection, const QMatrix4x4 &view, const QMatrix4x4 &model, const QVector3D &color) override;

private:
    void initialize(float size);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
};

#endif // SZESCIAN_H
