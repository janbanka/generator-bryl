#include "myglwidget.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MyGLWidget::MyGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_currentShape(None),
      m_program(nullptr),
      m_cuboidVBO(QOpenGLBuffer::VertexBuffer),
      m_cuboidIBO(QOpenGLBuffer::IndexBuffer),
      m_cylinderVBO(QOpenGLBuffer::VertexBuffer),
      m_cylinderIBO(QOpenGLBuffer::IndexBuffer),
      m_cylinderIndexCount(0),
      m_sphereVBO(QOpenGLBuffer::VertexBuffer),
      m_sphereIBO(QOpenGLBuffer::IndexBuffer),
      m_sphereIndexCount(0),
      m_coneVBO(QOpenGLBuffer::VertexBuffer),
      m_coneIBO(QOpenGLBuffer::IndexBuffer),
      m_coneIndexCount(0),
      m_rotation(0.0f)
{
}

MyGLWidget::~MyGLWidget()
{
    makeCurrent();
    m_cuboidVBO.destroy();
    m_cuboidIBO.destroy();
    m_cylinderVBO.destroy();
    m_cylinderIBO.destroy();
    m_sphereVBO.destroy();
    m_sphereIBO.destroy();
    m_coneVBO.destroy();
    m_coneIBO.destroy();
    delete m_program;
    doneCurrent();
}


void MyGLWidget::setCurrentShape(int shape)
{
    m_currentShape = shape;
    update();
}

void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // A dark blue background

    glEnable(GL_DEPTH_TEST);

    m_program = new QOpenGLShaderProgram(this);

    // Vertex Shader
    const char *vsrc =
        "attribute highp vec4 vertex;\n"
        "uniform highp mat4 projection;\n"
        "uniform highp mat4 view;\n"
        "uniform highp mat4 model;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = projection * view * model * vertex;\n"
        "}\n";
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);

    // Fragment Shader
    const char *fsrc =
        "uniform highp vec3 color;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);

    m_program->link();

    createCuboid();
    createCylinder();
    createSphere();
    createCone();

    m_view.setToIdentity();
    m_view.lookAt(QVector3D(0, 0, 5), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, QOverload<>::of(&MyGLWidget::update));
    m_timer->start(16); // ~60 FPS
}

void MyGLWidget::resizeGL(int w, int h)
{
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, float(w) / h, 0.01f, 100.0f);
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!m_program->bind())
        return;

    m_rotation += 1.0f;
    m_model.setToIdentity();
    m_model.rotate(m_rotation, 1.0f, 1.0f, 0.0f);


    m_program->setUniformValue("projection", m_projection);
    m_program->setUniformValue("view", m_view);
    m_program->setUniformValue("model", m_model);


    switch (m_currentShape) {
    case Cuboid:
        drawCuboid();
        break;
    case Cylinder:
        drawCylinder();
        break;
    case Sphere:
        drawSphere();
        break;
    case Cone:
        drawCone();
        break;
    default:
        // Draw nothing
        break;
    }

    m_program->release();
}

void MyGLWidget::createCuboid()
{
    QVector<QVector3D> vertices = {
        QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.5f, -0.5f,  0.5f),
        QVector3D( 0.5f,  0.5f,  0.5f), QVector3D(-0.5f,  0.5f,  0.5f),
        QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 0.5f, -0.5f, -0.5f),
        QVector3D( 0.5f,  0.5f, -0.5f), QVector3D(-0.5f,  0.5f, -0.5f)
    };

    QVector<GLushort> indices = {
        0, 1, 2, 2, 3, 0, // Front
        4, 5, 6, 6, 7, 4, // Back
        0, 4, 7, 7, 3, 0, // Left
        1, 5, 6, 6, 2, 1, // Right
        3, 2, 6, 6, 7, 3, // Top
        0, 1, 5, 5, 4, 0  // Bottom
    };

    m_cuboidVBO.create();
    m_cuboidVBO.bind();
    m_cuboidVBO.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));

    m_cuboidIBO.create();
    m_cuboidIBO.bind();
    m_cuboidIBO.allocate(indices.constData(), indices.size() * sizeof(GLushort));
}

void MyGLWidget::createCylinder()
{
    QVector<QVector3D> vertices;
    QVector<GLushort> indices;
    int segments = 32;
    float height = 1.0f;
    float radius = 0.5f;

    // Top center vertex
    vertices.append(QVector3D(0, height / 2, 0));
    // Top circle vertices
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(segments);
        vertices.append(QVector3D(cos(angle) * radius, height / 2, sin(angle) * radius));
    }
    // Bottom center vertex
    vertices.append(QVector3D(0, -height / 2, 0));
    // Bottom circle vertices
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(segments);
        vertices.append(QVector3D(cos(angle) * radius, -height / 2, sin(angle) * radius));
    }

    // Top cap indices
    for (int i = 1; i <= segments; ++i) {
        indices.append(0);
        indices.append(i);
        indices.append((i % segments) + 1);
    }

    // Bottom cap indices
    int bottomCenterIndex = segments + 1;
    int bottomStartIndex = bottomCenterIndex + 1;
    for (int i = 0; i < segments; ++i) {
        indices.append(bottomCenterIndex);
        indices.append(bottomStartIndex + i);
        indices.append(bottomStartIndex + ((i + 1) % segments));
    }

    // Side indices
    int topStartIndex = 1;
    for (int i = 0; i < segments; ++i) {
        int i0 = topStartIndex + i;
        int i1 = topStartIndex + (i + 1) % segments;
        int i2 = bottomStartIndex + i;
        int i3 = bottomStartIndex + (i + 1) % segments;

        indices.append(i0); indices.append(i2); indices.append(i1);
        indices.append(i1); indices.append(i2); indices.append(i3);
    }
    
    m_cylinderIndexCount = indices.size();

    m_cylinderVBO.create();
    m_cylinderVBO.bind();
    m_cylinderVBO.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));

    m_cylinderIBO.create();
    m_cylinderIBO.bind();
    m_cylinderIBO.allocate(indices.constData(), indices.size() * sizeof(GLushort));
}

void MyGLWidget::createSphere()
{
    QVector<QVector3D> vertices;
    QVector<GLushort> indices;
    int rings = 32;
    int sectors = 32;
    float radius = 0.75f;

    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);

    for(int r = 0; r < rings; r++) for(int s = 0; s < sectors; s++) {
        float const y = sin( -M_PI/2 + M_PI * r * R );
        float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
        float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

        vertices.append(QVector3D(x * radius, y * radius, z * radius));
    }

    for(int r = 0; r < rings-1; r++) for(int s = 0; s < sectors-1; s++) {
        indices.append(r * sectors + s);
        indices.append(r * sectors + (s+1));
        indices.append((r+1) * sectors + (s+1));

        indices.append((r+1) * sectors + (s+1));
        indices.append((r+1) * sectors + s);
        indices.append(r * sectors + s);
    }

    m_sphereIndexCount = indices.size();

    m_sphereVBO.create();
    m_sphereVBO.bind();
    m_sphereVBO.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));

    m_sphereIBO.create();
    m_sphereIBO.bind();
    m_sphereIBO.allocate(indices.constData(), indices.size() * sizeof(GLushort));
}

void MyGLWidget::createCone()
{
    QVector<QVector3D> vertices;
    QVector<GLushort> indices;
    int segments = 32;
    float height = 1.0f;
    float radius = 0.5f;

    // Apex vertex
    vertices.append(QVector3D(0, height / 2, 0));
    // Base center vertex
    vertices.append(QVector3D(0, -height / 2, 0));
    // Base circle vertices
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(segments);
        vertices.append(QVector3D(cos(angle) * radius, -height / 2, sin(angle) * radius));
    }

    // Base indices
    int baseCenterIndex = 1;
    int baseStartIndex = 2;
    for (int i = 0; i < segments; ++i) {
        indices.append(baseCenterIndex);
        indices.append(baseStartIndex + i);
        indices.append(baseStartIndex + ((i + 1) % segments));
    }

    // Side indices
    int apexIndex = 0;
    for (int i = 0; i < segments; ++i) {
        indices.append(apexIndex);
        indices.append(baseStartIndex + i);
        indices.append(baseStartIndex + ((i + 1) % segments));
    }

    m_coneIndexCount = indices.size();

    m_coneVBO.create();
    m_coneVBO.bind();
    m_coneVBO.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));

    m_coneIBO.create();
    m_coneIBO.bind();
    m_coneIBO.allocate(indices.constData(), indices.size() * sizeof(GLushort));
}

void MyGLWidget::drawCuboid()
{
    m_program->setUniformValue("color", QVector3D(1.0f, 0.0f, 0.0f)); // Red

    m_cuboidVBO.bind();
    m_cuboidIBO.bind();

    int vertexLocation = m_program->attributeLocation("vertex");
    m_program->enableAttributeArray(vertexLocation);
    m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);

    m_program->disableAttributeArray(vertexLocation);
}

void MyGLWidget::drawCylinder()
{
    m_program->setUniformValue("color", QVector3D(0.0f, 1.0f, 0.0f)); // Green

    m_cylinderVBO.bind();
    m_cylinderIBO.bind();

    int vertexLocation = m_program->attributeLocation("vertex");
    m_program->enableAttributeArray(vertexLocation);
    m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    glDrawElements(GL_TRIANGLES, m_cylinderIndexCount, GL_UNSIGNED_SHORT, nullptr);

    m_program->disableAttributeArray(vertexLocation);
}

void MyGLWidget::drawSphere()
{
    m_program->setUniformValue("color", QVector3D(0.0f, 0.0f, 1.0f)); // Blue

    m_sphereVBO.bind();
    m_sphereIBO.bind();

    int vertexLocation = m_program->attributeLocation("vertex");
    m_program->enableAttributeArray(vertexLocation);
    m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    glDrawElements(GL_TRIANGLES, m_sphereIndexCount, GL_UNSIGNED_SHORT, nullptr);

    m_program->disableAttributeArray(vertexLocation);
}

void MyGLWidget::drawCone()
{
    m_program->setUniformValue("color", QVector3D(1.0f, 1.0f, 0.0f)); // Yellow

    m_coneVBO.bind();
    m_coneIBO.bind();

    int vertexLocation = m_program->attributeLocation("vertex");
    m_program->enableAttributeArray(vertexLocation);
    m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    glDrawElements(GL_TRIANGLES, m_coneIndexCount, GL_UNSIGNED_SHORT, nullptr);

    m_program->disableAttributeArray(vertexLocation);
}
