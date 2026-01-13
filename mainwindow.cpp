#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "brylawidget.h"
#include <QButtonGroup>
#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow constructor: start";
    ui->setupUi(this);
    qDebug() << "MainWindow constructor: setupUi done";

    // Adjust layout stretch
    ui->horizontalLayout->setStretch(0, 5);
    ui->horizontalLayout->setStretch(1, 1);

    // Shape selection setup
    m_shapeButtonGroup = new QButtonGroup(this);
    m_shapeButtonGroup->addButton(ui->radioButtonWalec, BrylaWidget::Cylinder);
    m_shapeButtonGroup->addButton(ui->radioButtonKula, BrylaWidget::Sphere);
    m_shapeButtonGroup->addButton(ui->radioButtonProstopadloscian, BrylaWidget::Cuboid);
    m_shapeButtonGroup->addButton(ui->radioButtonStozek, BrylaWidget::Cone);
    m_shapeButtonGroup->addButton(ui->radioButtonTorus, BrylaWidget::Torus);
    m_shapeButtonGroup->addButton(ui->radioButtonKatenoida, BrylaWidget::Katenoida);
    m_shapeButtonGroup->addButton(ui->radioButtonHelikoida, BrylaWidget::Helikoida);
    m_shapeButtonGroup->addButton(ui->radioButtonWstegaMobiusa, BrylaWidget::WstegaMobiusa);
    m_shapeButtonGroup->addButton(ui->radioButtonMengerSponge, BrylaWidget::KostkaMengera);
    qDebug() << "MainWindow constructor: buttons added to group";
    
    // Color selection setup
    m_colors.append(QColor(Qt::white));
    m_colors.append(QColor(255, 165, 0)); // Orange
    m_colors.append(QColor(Qt::black));
    m_colors.append(QColor(Qt::blue));
    m_colors.append(QColor(Qt::yellow));
    m_colors.append(QColor(Qt::red));

    m_colorButtonGroup = new QButtonGroup(this);
    m_colorButtonGroup->addButton(ui->colorButton1, 0);
    m_colorButtonGroup->addButton(ui->colorButton2, 1);
    m_colorButtonGroup->addButton(ui->colorButton3, 2);
    m_colorButtonGroup->addButton(ui->colorButton4, 3);
    m_colorButtonGroup->addButton(ui->colorButton5, 4);
    m_colorButtonGroup->addButton(ui->colorButton6, 5);
    
    qDebug() << "MainWindow constructor: initializing UI";
    initializeUI();
    qDebug() << "MainWindow constructor: connecting signals";
    connectSignals();
    qDebug() << "MainWindow constructor: setting defaults";

    // Set default selections
    ui->colorButton1->setChecked(true);
    on_color_button_clicked(0);
    ui->radioButtonKula->setChecked(true);
    on_shape_button_clicked(BrylaWidget::Sphere);

    // Initialize auto-rotation state
    ui->brylaWidget->setAutoRotation(ui->checkBoxAutoRotate->isChecked());

    qDebug() << "MainWindow constructor: end";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeUI()
{
    qDebug() << "initializeUI: start";
    // Walec
    ui->doubleSpinBox_walec_radius->setRange(0.1, 10.0);
    ui->doubleSpinBox_walec_radius->setSingleStep(0.1);
    ui->doubleSpinBox_walec_radius->setValue(m_cylinderParams.radius);
    ui->doubleSpinBox_walec_height->setRange(0.1, 10.0);
    ui->doubleSpinBox_walec_height->setSingleStep(0.1);
    ui->doubleSpinBox_walec_height->setValue(m_cylinderParams.height);

    // Kula
    ui->doubleSpinBox_kula_radius->setRange(0.1, 10.0);
    ui->doubleSpinBox_kula_radius->setSingleStep(0.1);
    ui->doubleSpinBox_kula_radius->setValue(m_sphereParams.radius);

    // Prostopadloscian
    ui->doubleSpinBox_prostopadloscian_width->setRange(0.1, 10.0);
    ui->doubleSpinBox_prostopadloscian_width->setSingleStep(0.1);
    ui->doubleSpinBox_prostopadloscian_width->setValue(m_cuboidParams.width);
    ui->doubleSpinBox_prostopadloscian_height->setRange(0.1, 10.0);
    ui->doubleSpinBox_prostopadloscian_height->setSingleStep(0.1);
    ui->doubleSpinBox_prostopadloscian_height->setValue(m_cuboidParams.height);
    ui->doubleSpinBox_prostopadloscian_depth->setRange(0.1, 10.0);
    ui->doubleSpinBox_prostopadloscian_depth->setSingleStep(0.1);
    ui->doubleSpinBox_prostopadloscian_depth->setValue(m_cuboidParams.depth);

    // Stozek
    ui->doubleSpinBox_stozek_radius->setRange(0.1, 10.0);
    ui->doubleSpinBox_stozek_radius->setSingleStep(0.1);
    ui->doubleSpinBox_stozek_radius->setValue(m_coneParams.radius);
    ui->doubleSpinBox_stozek_height->setRange(0.1, 10.0);
    ui->doubleSpinBox_stozek_height->setSingleStep(0.1);
    ui->doubleSpinBox_stozek_height->setValue(m_coneParams.height);

    // Torus
    ui->doubleSpinBox_torus_major->setRange(0.1, 10.0);
    ui->doubleSpinBox_torus_major->setSingleStep(0.1);
    ui->doubleSpinBox_torus_major->setValue(m_torusParams.majorRadius);
    ui->doubleSpinBox_torus_minor->setRange(0.1, 10.0);
    ui->doubleSpinBox_torus_minor->setSingleStep(0.1);
    ui->doubleSpinBox_torus_minor->setValue(m_torusParams.minorRadius);
    
    // Menger Sponge
    ui->spinBox_menger_level->setRange(0, 4);
    ui->spinBox_menger_level->setValue(m_kostkaMengeraParams.level);

    // Katenoida
    ui->doubleSpinBox_katenoida_c->setRange(0.1, 10.0);
    ui->doubleSpinBox_katenoida_c->setSingleStep(0.1);
    ui->doubleSpinBox_katenoida_c->setValue(m_catenoidParams.c);
    ui->doubleSpinBox_katenoida_v_range->setRange(0.1, 10.0);
    ui->doubleSpinBox_katenoida_v_range->setSingleStep(0.1);
    ui->doubleSpinBox_katenoida_v_range->setValue(m_catenoidParams.v_range);
    
    // Helikoida
    ui->doubleSpinBox_helikoida_radius->setRange(0.1, 10.0);
    ui->doubleSpinBox_helikoida_radius->setSingleStep(0.1);
    ui->doubleSpinBox_helikoida_radius->setValue(m_helicoidParams.radius);
    ui->doubleSpinBox_helikoida_rotations->setRange(0.1, 10.0);
    ui->doubleSpinBox_helikoida_rotations->setSingleStep(0.1);
    ui->doubleSpinBox_helikoida_rotations->setValue(m_helicoidParams.rotations);
    ui->doubleSpinBox_helikoida_alpha->setRange(0.1, 10.0);
    ui->doubleSpinBox_helikoida_alpha->setSingleStep(0.1);
    ui->doubleSpinBox_helikoida_alpha->setValue(m_helicoidParams.alpha);

    // Wstega Mobiusa
    ui->doubleSpinBox_wstega_radius->setRange(0.1, 10.0);
    ui->doubleSpinBox_wstega_radius->setSingleStep(0.1);
    ui->doubleSpinBox_wstega_radius->setValue(m_mobiusParams.radius);
    ui->doubleSpinBox_wstega_width->setRange(0.1, 10.0);
    ui->doubleSpinBox_wstega_width->setSingleStep(0.1);
    ui->doubleSpinBox_wstega_width->setValue(m_mobiusParams.width);
    qDebug() << "initializeUI: end";
}

void MainWindow::connectSignals()
{
    qDebug() << "connectSignals: start";
    connect(m_shapeButtonGroup, SIGNAL(idClicked(int)), this, SLOT(on_shape_button_clicked(int)));
    connect(m_colorButtonGroup, SIGNAL(idClicked(int)), this, SLOT(on_color_button_clicked(int)));

    // Walec
    connect(ui->doubleSpinBox_walec_radius, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_walec_height, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));

    // Kula
    connect(ui->doubleSpinBox_kula_radius, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    
    // Prostopadloscian
    connect(ui->doubleSpinBox_prostopadloscian_width, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_prostopadloscian_height, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_prostopadloscian_depth, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    
    // Stozek
    connect(ui->doubleSpinBox_stozek_radius, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_stozek_height, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));

    // Torus
    connect(ui->doubleSpinBox_torus_major, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_torus_minor, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));

    // Menger Sponge
    connect(ui->spinBox_menger_level, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));

    // Katenoida
    connect(ui->doubleSpinBox_katenoida_c, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_katenoida_v_range, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));

    // Helikoida
    connect(ui->doubleSpinBox_helikoida_radius, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_helikoida_rotations, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_helikoida_alpha, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));

    // Wstega Mobiusa
    connect(ui->doubleSpinBox_wstega_radius, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_wstega_width, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));

    // Auto-rotacja
    connect(ui->checkBoxAutoRotate, &QCheckBox::toggled, ui->brylaWidget, &BrylaWidget::setAutoRotation);
    
    // Wireframe
    connect(ui->checkBoxWireframe, &QCheckBox::toggled, ui->brylaWidget, &BrylaWidget::setWireframe);

    // Reset Camera
    connect(ui->pushButtonResetCamera, &QPushButton::clicked, ui->brylaWidget, &BrylaWidget::resetCamera);

    qDebug() << "connectSignals: end";
}

void MainWindow::on_shape_button_clicked(int id)
{
    qDebug() << "on_shape_button_clicked: id =" << id;
    ui->parameterStack->setCurrentIndex(id-1); // Assumes enum order matches stack widget order
    regenerateShape(static_cast<BrylaWidget::Shape>(id));
}

void MainWindow::on_color_button_clicked(int id)
{
    if(id >= 0 && id < m_colors.size()) {
        ui->brylaWidget->setShapeColor(m_colors[id]);
    }
}

void MainWindow::on_params_changed()
{
    // Update the parameter structs from the UI
    BrylaWidget::Shape currentShape = static_cast<BrylaWidget::Shape>(m_shapeButtonGroup->checkedId());

    switch(currentShape)
    {
        case BrylaWidget::Cylinder:
            m_cylinderParams.radius = ui->doubleSpinBox_walec_radius->value();
            m_cylinderParams.height = ui->doubleSpinBox_walec_height->value();
            m_cylinderParams.sides = 64; // Default high quality
            break;
        case BrylaWidget::Sphere:
            m_sphereParams.radius = ui->doubleSpinBox_kula_radius->value();
            m_sphereParams.rings = 64; // Default high quality
            m_sphereParams.sectors = 64; // Default high quality
            break;
        case BrylaWidget::Cuboid:
            m_cuboidParams.width = ui->doubleSpinBox_prostopadloscian_width->value();
            m_cuboidParams.height = ui->doubleSpinBox_prostopadloscian_height->value();
            m_cuboidParams.depth = ui->doubleSpinBox_prostopadloscian_depth->value();
            break;
        case BrylaWidget::Cone:
            m_coneParams.radius = ui->doubleSpinBox_stozek_radius->value();
            m_coneParams.height = ui->doubleSpinBox_stozek_height->value();
            m_coneParams.sides = 64; // Default high quality
            break;
        case BrylaWidget::Torus:
            m_torusParams.majorRadius = ui->doubleSpinBox_torus_major->value();
            m_torusParams.minorRadius = ui->doubleSpinBox_torus_minor->value();
            m_torusParams.majorSegments = 64; // Default high quality
            m_torusParams.minorSegments = 64; // Default high quality
            break;
        case BrylaWidget::KostkaMengera:
            m_kostkaMengeraParams.level = ui->spinBox_menger_level->value();
            break;
        case BrylaWidget::Katenoida:
            m_catenoidParams.c = ui->doubleSpinBox_katenoida_c->value();
            m_catenoidParams.v_range = ui->doubleSpinBox_katenoida_v_range->value();
            m_catenoidParams.u_segments = 64; // Default high quality
            m_catenoidParams.v_segments = 64; // Default high quality
            break;
        case BrylaWidget::Helikoida:
            m_helicoidParams.radius = ui->doubleSpinBox_helikoida_radius->value();
            m_helicoidParams.rotations = ui->doubleSpinBox_helikoida_rotations->value();
            m_helicoidParams.alpha = ui->doubleSpinBox_helikoida_alpha->value();
            m_helicoidParams.radius_segments = 32; // Default high quality
            m_helicoidParams.theta_segments = 128; // Default high quality
            break;
        case BrylaWidget::WstegaMobiusa:
            m_mobiusParams.radius = ui->doubleSpinBox_wstega_radius->value();
            m_mobiusParams.width = ui->doubleSpinBox_wstega_width->value();
            m_mobiusParams.u_segments = 128; // Default high quality
            m_mobiusParams.v_segments = 32; // Default high quality
            break;
        default:
            break;
    }
    regenerateShape(currentShape);
}

void MainWindow::regenerateShape(BrylaWidget::Shape shape)
{
    qDebug() << "regenerateShape: shape id =" << shape;
    
    QString formula;
    switch(shape)
    {
        case BrylaWidget::Cylinder:
            ui->brylaWidget->setCurrentShape(shape, m_cylinderParams);
            formula = "<b>Walec:</b><br>"
                      "x = r cos(θ), y = v, z = r sin(θ)<br>"
                      "<small><i>Legenda: r – promień, v – wysokość, θ – kąt obrotu</i></small>";
            break;
        case BrylaWidget::Sphere:
            ui->brylaWidget->setCurrentShape(shape, m_sphereParams);
            formula = "<b>Kula:</b><br>"
                      "x = r sin(φ) cos(θ), y = r sin(φ) sin(θ), z = r cos(φ)<br>"
                      "<small><i>Legenda: r – promień, θ – azymut, φ – kąt pionowy</i></small>";
            break;
        case BrylaWidget::Cuboid:
            ui->brylaWidget->setCurrentShape(shape, m_cuboidParams);
            formula = "<b>Prostopadłościan:</b><br>"
                      "|x| ≤ w/2, |y| ≤ h/2, |z| ≤ d/2<br>"
                      "<small><i>Legenda: w – szerokość, h – wysokość, d – głębokość</i></small>";
            break;
        case BrylaWidget::Cone:
            ui->brylaWidget->setCurrentShape(shape, m_coneParams);
            formula = "<b>Stożek:</b><br>"
                      "x = (1 - u/h) r cos(θ), y = u, z = (1 - u/h) r sin(θ)<br>"
                      "<small><i>Legenda: r – promień podstawy, h – wysokość, u – wys. bieżąca, θ – kąt</i></small>";
            break;
        case BrylaWidget::Torus:
            ui->brylaWidget->setCurrentShape(shape, m_torusParams);
            formula = "<b>Torus:</b><br>"
                      "x = (R + r cos(θ)) cos(φ), y = (R + r cos(θ)) sin(φ), z = r sin(θ)<br>"
                      "<small><i>Legenda: R – promień główny, r – promień rury, φ, θ – kąty obrotu</i></small>";
            break;
        case BrylaWidget::KostkaMengera:
             ui->brylaWidget->setCurrentShape(shape, m_kostkaMengeraParams);
             formula = "<b>Kostka Mengera:</b><br>"
                       "Fraktal powstający przez rekurencyjne usuwanie środkowych części sześcianu<br>"
                       "<small><i>Legenda: n – stopień rekurencji</i></small>";
            break;
        case BrylaWidget::Katenoida:
            ui->brylaWidget->setCurrentShape(shape, m_catenoidParams);
            formula = "<b>Katenoida:</b><br>"
                      "x = c cosh(v/c) cos(u), y = c cosh(v/c) sin(u), z = v<br>"
                      "<small><i>Legenda: c – promień przewężenia, u – kąt obrotu, v – wysokość</i></small>";
            break;
        case BrylaWidget::Helikoida:
            ui->brylaWidget->setCurrentShape(shape, m_helicoidParams);
            formula = "<b>Helikoida:</b><br>"
                      "x = ρ cos(θ), y = ρ sin(θ), z = αθ<br>"
                      "<small><i>Legenda: ρ – odl. od osi, θ – kąt, α – współczynnik skoku</i></small>";
            break;
        case BrylaWidget::WstegaMobiusa:
            ui->brylaWidget->setCurrentShape(shape, m_mobiusParams);
            formula = "<b>Wstęga Möbiusa:</b><br>"
                      "x = (R + w cos(θ/2)) cos(θ), y = (R + w cos(θ/2)) sin(θ), z = w sin(θ/2)<br>"
                      "<small><i>Legenda: R – promień pętli, w – szerokość wstęgi, θ – kąt obrotu</i></small>";
            break;
        default:
             ui->brylaWidget->setCurrentShape(BrylaWidget::None);
             formula = "";
            break;
    }
    ui->labelFormula->setText(formula);
}
