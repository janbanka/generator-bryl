#ifndef WSTEGAMOBIUSA_H
#define WSTEGAMOBIUSA_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include "Vertex.h"

class WstegaMobiusa : public Bryla, protected QOpenGLFunctions {
public:
    WstegaMobiusa(float radius, float width, int u_segments, int v_segments);
    ~WstegaMobiusa();
    
    void draw(QOpenGLShaderProgram *program) override;

private:
    void initialize(float radius, float width, int u_segments, int v_segments);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    int m_indexCount;
};

#endif // WSTEGAMOBIUSA_H
