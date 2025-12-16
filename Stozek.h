#ifndef STOZEK_H
#define STOZEK_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

class Stozek : public Bryla, protected QOpenGLFunctions {
public:
    Stozek(float radius, float height, int sides);
    ~Stozek();
    
    void draw(QOpenGLShaderProgram *program, const QMatrix4x4 &projection, const QMatrix4x4 &view, const QMatrix4x4 &model, const QVector3D &color) override;

private:
    void initialize(float radius, float height, int sides);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    int m_indexCount;
};

#endif // STOZEK_H
