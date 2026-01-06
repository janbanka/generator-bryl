#include <variant>
#include "brylawidget.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <cmath>
#include "Walec.h"
#include "Kula.h"
#include "Szescian.h"
#include "Stozek.h"
#include "Torus.h"
#include "Katenoida.h"
#include "Helikoida.h"
#include "WstegaMobiusa.h"
#include "MengerSponge.h"

#include <QDebug>

BrylaWidget::BrylaWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_program(nullptr),
      m_shape(nullptr),
      m_shapeColor(0.5f, 0.5f, 0.8f), // Default color
      m_translation(0.0f, 0.0f, 0.0f),
      m_cameraDistance(5.0f),
      m_isInitialized(false),
      m_pendingShape(None)
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

void BrylaWidget::setCurrentShape(Shape shape, const ShapeParameters& params)
{
    qDebug() << "BrylaWidget::setCurrentShape (with params): shape =" << shape;

    if (!m_isInitialized) {
        qDebug() << "BrylaWidget not initialized. Deferring shape creation.";
        m_pendingShape = shape;
        m_pendingParams = params;
        return;
    }

    makeCurrent();
    
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, CylinderParameters>)
            m_shape = std::make_unique<::Walec>(arg.radius, arg.height, arg.sides);
        else if constexpr (std::is_same_v<T, SphereParameters>)
            m_shape = std::make_unique<::Kula>(arg.radius, arg.rings, arg.sectors);
        else if constexpr (std::is_same_v<T, CuboidParameters>)
            m_shape = std::make_unique<::Szescian>(arg.size);
        else if constexpr (std::is_same_v<T, ConeParameters>)
            m_shape = std::make_unique<::Stozek>(arg.radius, arg.height, arg.sides);
        else if constexpr (std::is_same_v<T, TorusParameters>)
            m_shape = std::make_unique<::Torus>(arg.majorRadius, arg.minorRadius, arg.majorSegments, arg.minorSegments);
        else if constexpr (std::is_same_v<T, CatenoidParameters>)
            m_shape = std::make_unique<::Katenoida>(arg.c, arg.v_range, arg.u_segments, arg.v_segments);
        else if constexpr (std::is_same_v<T, HelicoidParameters>)
            m_shape = std::make_unique<::Helikoida>(arg.radius, arg.rotations, arg.alpha, arg.radius_segments, arg.theta_segments);
        else if constexpr (std::is_same_v<T, MobiusStripParameters>)
            m_shape = std::make_unique<::WstegaMobiusa>(arg.radius, arg.width, arg.u_segments, arg.v_segments);
        else if constexpr (std::is_same_v<T, MengerSpongeParameters>)
            m_shape = std::make_unique<::MengerSponge>(arg.level);
    }, params);

    doneCurrent();
    update();
}

void BrylaWidget::setCurrentShape(Shape shape)
{
    if (shape == None) {
        if (!m_isInitialized) {
             m_pendingShape = None;
             return;
        }
        makeCurrent();
        m_shape.reset();
        doneCurrent();
        update();
    }
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
    m_isInitialized = true;
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    m_program = new QOpenGLShaderProgram(this);

    const char *vsrc =
        "attribute highp vec3 position;\n"
        "attribute highp vec3 normal;\n"
        "uniform highp mat4 projection;\n"
        "uniform highp mat4 view;\n"
        "uniform highp mat4 model;\n"
        "uniform highp mat3 normalMatrix;\n"
        "varying highp vec3 v_worldPosition;\n"
        "varying highp vec3 v_worldNormal;\n"
        "void main(void)\n"
        "{\n"
        "    vec4 worldPos = model * vec4(position, 1.0);\n"
        "    v_worldPosition = worldPos.xyz;\n"
        "    v_worldNormal = normalize(normalMatrix * normal);\n"
        "    gl_Position = projection * view * worldPos;\n"
        "}\n";
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);

    const char *fsrc =
        "uniform highp vec3 objectColor;\n"
        "uniform highp vec3 lightColor;\n"
        "uniform highp vec3 lightPos;\n"
        "uniform highp vec3 viewPos;\n"
        "varying highp vec3 v_worldPosition;\n"
        "varying highp vec3 v_worldNormal;\n"
        "void main(void)\n"
        "{\n"
        "    vec3 norm = normalize(v_worldNormal);\n"
        "    vec3 viewDir = normalize(viewPos - v_worldPosition);\n"
        "    if (dot(norm, viewDir) < 0.0) {\n"
        "       norm = -norm;\n"
        "    }\n"
        "\n"
        "    // Ambient\n"
        "    float ambientStrength = 0.2;\n"
        "    vec3 ambient = ambientStrength * lightColor;\n"
        "\n"
        "    // Diffuse\n"
        "    vec3 lightDir = normalize(lightPos - v_worldPosition);\n"
        "    float diff = max(dot(norm, lightDir), 0.0);\n"
        "    vec3 diffuse = diff * lightColor;\n"
        "\n"
        "    // Specular\n"
        "    float specularStrength = 0.5;\n"
        "    vec3 reflectDir = reflect(-lightDir, norm);\n"
        "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);\n"
        "    vec3 specular = specularStrength * spec * lightColor;\n"
        "\n"
        "    vec3 result = (ambient + diffuse + specular) * objectColor;\n"
        "    gl_FragColor = vec4(result, 1.0);\n"
        "}\n";
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);

    m_program->link();

    if (m_pendingShape != None) {
        qDebug() << "initializeGL: Creating pending shape:" << m_pendingShape;
        setCurrentShape(m_pendingShape, m_pendingParams);
    }
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

    QVector3D viewPos(0, 0, m_cameraDistance);
    m_view.setToIdentity();
    m_view.lookAt(viewPos, QVector3D(0, 0, 0), QVector3D(0, 1, 0));

    m_model.setToIdentity();
    m_model.translate(m_translation);
    m_model.rotate(m_rotation);

    m_program->setUniformValue("projection", m_projection);
    m_program->setUniformValue("view", m_view);
    m_program->setUniformValue("model", m_model);
    m_program->setUniformValue("normalMatrix", m_model.normalMatrix());
    m_program->setUniformValue("objectColor", m_shapeColor);
    m_program->setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
    m_program->setUniformValue("lightPos", QVector3D(2.0f, 2.0f, 5.0f));
    m_program->setUniformValue("viewPos", viewPos);


    if (m_shape)
    {
        m_shape->draw(m_program);
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
