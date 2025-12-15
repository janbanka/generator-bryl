#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QTimer>


class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    enum Shape {
        None,
        Cylinder,
        Sphere,
        Cuboid,
        Cone
    };
    Q_ENUM(Shape)

    explicit MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

public slots:
    void setCurrentShape(int shape);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void createCuboid();
    void drawCuboid();

    void createCylinder();
    void drawCylinder();

    void createSphere();
    void drawSphere();

    void createCone();
    void drawCone();


    int m_currentShape;
    QOpenGLShaderProgram *m_program;

    QMatrix4x4 m_projection;
    QMatrix4x4 m_view;
    QMatrix4x4 m_model;

    QOpenGLBuffer m_cuboidVBO;
    QOpenGLBuffer m_cuboidIBO;

    QOpenGLBuffer m_cylinderVBO;
    QOpenGLBuffer m_cylinderIBO;
    int m_cylinderIndexCount;

    QOpenGLBuffer m_sphereVBO;
    QOpenGLBuffer m_sphereIBO;
    int m_sphereIndexCount;

    QOpenGLBuffer m_coneVBO;
    QOpenGLBuffer m_coneIBO;
    int m_coneIndexCount;

    QTimer *m_timer;
    float m_rotation;
};

#endif // MYGLWIDGET_H
