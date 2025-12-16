#ifndef BRYLA_H
#define BRYLA_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class Bryla {
public:
    virtual ~Bryla() = default;

    virtual void draw(QOpenGLShaderProgram *program, const QMatrix4x4 &projection, const QMatrix4x4 &view, const QMatrix4x4 &model, const QVector3D &color) = 0;
};

#endif // BRYLA_H