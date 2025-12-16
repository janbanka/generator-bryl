#ifndef BRYLAWIDGET_H
#define BRYLAWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QQuaternion>
#include <memory>
#include "Bryla.h"


class BrylaWidget : public QOpenGLWidget, protected QOpenGLFunctions
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

    explicit BrylaWidget(QWidget *parent = nullptr);
    ~BrylaWidget();

public slots:
    void setCurrentShape(Shape shape);
    void setShapeColor(const QColor &color);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QOpenGLShaderProgram *m_program;

    QMatrix4x4 m_projection;
    QMatrix4x4 m_view;
    QMatrix4x4 m_model;

    std::unique_ptr<Bryla> m_shape;
    QVector3D m_shapeColor;

    QPoint m_lastPos;
    QQuaternion m_rotation;
    QVector3D m_translation;
    float m_cameraDistance;
};

#endif // BRYLAWIDGET_H

