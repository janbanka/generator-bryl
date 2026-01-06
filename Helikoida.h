#ifndef HELIKoida_H
#define HELIKoida_H

#include "Bryla.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include "Vertex.h"

class Helikoida : public Bryla, protected QOpenGLFunctions {
public:
    Helikoida(float radius, float rotations, float alpha, int radius_segments, int theta_segments);
    ~Helikoida();
    
    void draw(QOpenGLShaderProgram *program) override;

private:
    void initialize(float radius, float rotations, float alpha, int radius_segments, int theta_segments);

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    int m_indexCount;
};

#endif // HELIKoida_H
