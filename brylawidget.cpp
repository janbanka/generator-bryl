#include "brylawidget.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <cmath>
#include "Walec.h"
#include "Kula.h"
#include "Szescian.h"
#include "Stozek.h"

BrylaWidget::BrylaWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_program(nullptr),
      m_shape(nullptr),
      m_shapeColor(0.5f, 0.5f, 0.8f), // Default color
      m_translation(0.0f, 0.0f, 0.0f),
      m_cameraDistance(5.0f)
{
    m_rotation = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, -30.0f);
}

BrylaWidget::~BrylaWidget()
{
    makeCurrent();
    // m_shape is managed by unique_ptr
    delete m_program;
    doneCurrent();
}

void BrylaWidget::setCurrentShape(Shape shape)
{
    makeCurrent();
    switch (shape)
    {
    case Cylinder:
        m_shape = std::make_unique<Walec>(0.5f, 1.0f, 32);
        break;
    case Sphere:
        m_shape = std::make_unique<Kula>(0.75f, 32, 32);
        break;
    case Cuboid:
        m_shape = std::make_unique<Szescian>(1.0f);
        break;
    case Cone:
        m_shape = std::make_unique<Stozek>(0.5f, 1.0f, 32);
        break;
    case None:
    default:
        m_shape.reset();
        break;
    }
    doneCurrent();
    update();
}


void BrylaWidget::setShapeColor(const QColor &color)
{
    m_shapeColor.setX(color.redF());
    m_shapeColor.setY(color.greenF());
    m_shapeColor.setZ(color.blueF());
    update();
}

void BrylaWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    m_program = new QOpenGLShaderProgram(this);

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

    const char *fsrc =
        "uniform highp vec3 color;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);

    m_program->link();
}

void BrylaWidget::resizeGL(int w, int h)
{
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, float(w) / h, 0.01f, 100.0f);
}

void BrylaWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!m_program->bind())
        return;
    
    // Set up view matrix
    m_view.setToIdentity();
    m_view.lookAt(QVector3D(0, 0, m_cameraDistance), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

    m_model.setToIdentity();
    m_model.translate(m_translation);
    m_model.rotate(m_rotation);

    if (m_shape)
    {
        // Draw solid shape
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Ensure solid fill mode
        m_shape->draw(m_program, m_projection, m_view, m_model, m_shapeColor);
    }

    m_program->release();
}

void BrylaWidget::mousePressEvent(QMouseEvent *event)
{
    // Store the initial position for both left and right button drags
    m_lastPos = event->pos();
}

void BrylaWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->pos().x() - m_lastPos.x();
    int dy = event->pos().y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        // Rotation
        float rotationSpeed = 0.5f;
        QVector3D axis(dy, dx, 0.0);
        float angle = rotationSpeed * QVector2D(dx, dy).length();

        m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;
        
        update();
    } else if (event->buttons() & Qt::RightButton) {
        // Translation
        float translationSpeed = 0.01f;
        m_translation.setX(m_translation.x() + dx * translationSpeed);
        m_translation.setY(m_translation.y() - dy * translationSpeed); // Y is inverted in screen coords

        update();
    }
    
    m_lastPos = event->pos();
}

void BrylaWidget::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y();
    float zoomSpeed = 0.005f;

    if (delta > 0) {
        m_cameraDistance -= delta * zoomSpeed;
    } else {
        m_cameraDistance -= delta * zoomSpeed;
    }

    // Clamp the distance to avoid going too close or too far
    if (m_cameraDistance < 1.0f) m_cameraDistance = 1.0f;
    if (m_cameraDistance > 20.0f) m_cameraDistance = 20.0f;

    update();
}
