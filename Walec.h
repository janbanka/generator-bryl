#ifndef WALEC_H
#define WALEC_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <vector>

class Walec : public Bryla, protected QOpenGLFunctions {
public:
    Walec(float radius, float height, int sides);
    ~Walec();
    
    void draw(QOpenGLShaderProgram *program, const QMatrix4x4 &projection, const QMatrix4x4 &view, const QMatrix4x4 &model, const QVector3D &color) override;

private:
    void initialize(float radius, float height, int sides);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    int m_indexCount;
};

#endif // WALEC_H