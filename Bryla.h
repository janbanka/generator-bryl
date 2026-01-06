#ifndef BRYLA_H
#define BRYLA_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class Bryla {
public:
    virtual ~Bryla() = default;

    virtual void draw(QOpenGLShaderProgram *program) = 0;
};

#endif // BRYLA_H