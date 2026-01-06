#ifndef KATENOIDA_H
#define KATENOIDA_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include "Vertex.h"

class Katenoida : public Bryla, protected QOpenGLFunctions {
public:
    Katenoida(float c, float v_range, int u_segments, int v_segments);
    ~Katenoida();
    
    void draw(QOpenGLShaderProgram *program) override;

private:
    void initialize(float c, float v_range, int u_segments, int v_segments);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    int m_indexCount;
};

#endif // KATENOIDA_H
