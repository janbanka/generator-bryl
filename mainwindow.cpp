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
    m_shapeButtonGroup->addButton(ui->radioButtonMengerSponge, BrylaWidget::MengerSponge);
    qDebug() << "MainWindow constructor: buttons added to group";
    
    // Color selection setup
    m_colors.append(QColor(Qt::red));
    m_colors.append(QColor(Qt::green));
    m_colors.append(QColor(Qt::blue));
    m_colors.append(QColor(Qt::yellow));
    m_colors.append(QColor(Qt::cyan));
    m_colors.append(QColor(Qt::magenta));
    m_colors.append(QColor(Qt::white));
    m_colors.append(QColor(Qt::gray));
    m_colors.append(QColor(255, 165, 0)); // Orange
    m_colors.append(QColor(128, 0, 128)); // Purple

    m_colorButtonGroup = new QButtonGroup(this);
    m_colorButtonGroup->addButton(ui->colorButton1, 0);
    m_colorButtonGroup->addButton(ui->colorButton2, 1);
    m_colorButtonGroup->addButton(ui->colorButton3, 2);
    m_colorButtonGroup->addButton(ui->colorButton4, 3);
    m_colorButtonGroup->addButton(ui->colorButton5, 4);
    m_colorButtonGroup->addButton(ui->colorButton6, 5);
    m_colorButtonGroup->addButton(ui->colorButton7, 6);
    m_colorButtonGroup->addButton(ui->colorButton8, 7);
    m_colorButtonGroup->addButton(ui->colorButton9, 8);
    m_colorButtonGroup->addButton(ui->colorButton10, 9);
    
    qDebug() << "MainWindow constructor: initializing UI";
    initializeUI();
    qDebug() << "MainWindow constructor: connecting signals";
    connectSignals();
    qDebug() << "MainWindow constructor: setting defaults";

    // Set default selections
    ui->colorButton3->setChecked(true);
    on_color_button_clicked(2);
    ui->radioButtonKula->setChecked(true);
    on_shape_button_clicked(BrylaWidget::Sphere);
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
    ui->spinBox_walec_sides->setRange(3, 256);
    ui->spinBox_walec_sides->setValue(m_cylinderParams.sides);

    // Kula
    ui->doubleSpinBox_kula_radius->setRange(0.1, 10.0);
    ui->doubleSpinBox_kula_radius->setSingleStep(0.1);
    ui->doubleSpinBox_kula_radius->setValue(m_sphereParams.radius);
    ui->spinBox_kula_rings->setRange(3, 256);
    ui->spinBox_kula_rings->setValue(m_sphereParams.rings);
    ui->spinBox_kula_sectors->setRange(3, 256);
    ui->spinBox_kula_sectors->setValue(m_sphereParams.sectors);

    // Prostopadloscian
    ui->doubleSpinBox_prostopadloscian_size->setRange(0.1, 10.0);
    ui->doubleSpinBox_prostopadloscian_size->setSingleStep(0.1);
    ui->doubleSpinBox_prostopadloscian_size->setValue(m_cuboidParams.size);

    // Stozek
    ui->doubleSpinBox_stozek_radius->setRange(0.1, 10.0);
    ui->doubleSpinBox_stozek_radius->setSingleStep(0.1);
    ui->doubleSpinBox_stozek_radius->setValue(m_coneParams.radius);
    ui->doubleSpinBox_stozek_height->setRange(0.1, 10.0);
    ui->doubleSpinBox_stozek_height->setSingleStep(0.1);
    ui->doubleSpinBox_stozek_height->setValue(m_coneParams.height);
    ui->spinBox_stozek_sides->setRange(3, 256);
    ui->spinBox_stozek_sides->setValue(m_coneParams.sides);

    // Torus
    ui->doubleSpinBox_torus_major->setRange(0.1, 10.0);
    ui->doubleSpinBox_torus_major->setSingleStep(0.1);
    ui->doubleSpinBox_torus_major->setValue(m_torusParams.majorRadius);
    ui->doubleSpinBox_torus_minor->setRange(0.1, 10.0);
    ui->doubleSpinBox_torus_minor->setSingleStep(0.1);
    ui->doubleSpinBox_torus_minor->setValue(m_torusParams.minorRadius);
    ui->spinBox_torus_major_seg->setRange(3, 256);
    ui->spinBox_torus_major_seg->setValue(m_torusParams.majorSegments);
    ui->spinBox_torus_minor_seg->setRange(3, 256);
    ui->spinBox_torus_minor_seg->setValue(m_torusParams.minorSegments);
    
    // Menger Sponge
    ui->spinBox_menger_level->setRange(0, 3);
    ui->spinBox_menger_level->setValue(m_mengerParams.level);

    // Katenoida
    ui->doubleSpinBox_katenoida_c->setRange(0.1, 10.0);
    ui->doubleSpinBox_katenoida_c->setSingleStep(0.1);
    ui->doubleSpinBox_katenoida_c->setValue(m_catenoidParams.c);
    ui->doubleSpinBox_katenoida_v_range->setRange(0.1, 10.0);
    ui->doubleSpinBox_katenoida_v_range->setSingleStep(0.1);
    ui->doubleSpinBox_katenoida_v_range->setValue(m_catenoidParams.v_range);
    ui->spinBox_katenoida_u_seg->setRange(3, 256);
    ui->spinBox_katenoida_u_seg->setValue(m_catenoidParams.u_segments);
    ui->spinBox_katenoida_v_seg->setRange(3, 256);
    ui->spinBox_katenoida_v_seg->setValue(m_catenoidParams.v_segments);
    
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
    ui->spinBox_helikoida_radius_seg->setRange(3, 256);
    ui->spinBox_helikoida_radius_seg->setValue(m_helicoidParams.radius_segments);
    ui->spinBox_helikoida_theta_seg->setRange(3, 256);
    ui->spinBox_helikoida_theta_seg->setValue(m_helicoidParams.theta_segments);

    // Wstega Mobiusa
    ui->doubleSpinBox_wstega_radius->setRange(0.1, 10.0);
    ui->doubleSpinBox_wstega_radius->setSingleStep(0.1);
    ui->doubleSpinBox_wstega_radius->setValue(m_mobiusParams.radius);
    ui->doubleSpinBox_wstega_width->setRange(0.1, 10.0);
    ui->doubleSpinBox_wstega_width->setSingleStep(0.1);
    ui->doubleSpinBox_wstega_width->setValue(m_mobiusParams.width);
    ui->spinBox_wstega_u_seg->setRange(3, 256);
    ui->spinBox_wstega_u_seg->setValue(m_mobiusParams.u_segments);
    ui->spinBox_wstega_v_seg->setRange(3, 256);
    ui->spinBox_wstega_v_seg->setValue(m_mobiusParams.v_segments);
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
    connect(ui->spinBox_walec_sides, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));

    // Kula
    connect(ui->doubleSpinBox_kula_radius, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->spinBox_kula_rings, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));
    connect(ui->spinBox_kula_sectors, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));
    
    // Prostopadloscian
    connect(ui->doubleSpinBox_prostopadloscian_size, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    
    // Stozek
    connect(ui->doubleSpinBox_stozek_radius, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_stozek_height, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->spinBox_stozek_sides, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));

    // Torus
    connect(ui->doubleSpinBox_torus_major, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_torus_minor, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->spinBox_torus_major_seg, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));
    connect(ui->spinBox_torus_minor_seg, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));

    // Menger Sponge
    connect(ui->spinBox_menger_level, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));

    // Katenoida
    connect(ui->doubleSpinBox_katenoida_c, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_katenoida_v_range, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->spinBox_katenoida_u_seg, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));
    connect(ui->spinBox_katenoida_v_seg, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));

    // Helikoida
    connect(ui->doubleSpinBox_helikoida_radius, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_helikoida_rotations, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_helikoida_alpha, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->spinBox_helikoida_radius_seg, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));
    connect(ui->spinBox_helikoida_theta_seg, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));

    // Wstega Mobiusa
    connect(ui->doubleSpinBox_wstega_radius, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->doubleSpinBox_wstega_width, SIGNAL(valueChanged(double)), this, SLOT(on_params_changed()));
    connect(ui->spinBox_wstega_u_seg, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));
    connect(ui->spinBox_wstega_v_seg, SIGNAL(valueChanged(int)), this, SLOT(on_params_changed()));
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
            m_cylinderParams.sides = ui->spinBox_walec_sides->value();
            break;
        case BrylaWidget::Sphere:
            m_sphereParams.radius = ui->doubleSpinBox_kula_radius->value();
            m_sphereParams.rings = ui->spinBox_kula_rings->value();
            m_sphereParams.sectors = ui->spinBox_kula_sectors->value();
            break;
        case BrylaWidget::Cuboid:
            m_cuboidParams.size = ui->doubleSpinBox_prostopadloscian_size->value();
            break;
        case BrylaWidget::Cone:
            m_coneParams.radius = ui->doubleSpinBox_stozek_radius->value();
            m_coneParams.height = ui->doubleSpinBox_stozek_height->value();
            m_coneParams.sides = ui->spinBox_stozek_sides->value();
            break;
        case BrylaWidget::Torus:
            m_torusParams.majorRadius = ui->doubleSpinBox_torus_major->value();
            m_torusParams.minorRadius = ui->doubleSpinBox_torus_minor->value();
            m_torusParams.majorSegments = ui->spinBox_torus_major_seg->value();
            m_torusParams.minorSegments = ui->spinBox_torus_minor_seg->value();
            break;
        case BrylaWidget::MengerSponge:
            m_mengerParams.level = ui->spinBox_menger_level->value();
            break;
        case BrylaWidget::Katenoida:
            m_catenoidParams.c = ui->doubleSpinBox_katenoida_c->value();
            m_catenoidParams.v_range = ui->doubleSpinBox_katenoida_v_range->value();
            m_catenoidParams.u_segments = ui->spinBox_katenoida_u_seg->value();
            m_catenoidParams.v_segments = ui->spinBox_katenoida_v_seg->value();
            break;
        case BrylaWidget::Helikoida:
            m_helicoidParams.radius = ui->doubleSpinBox_helikoida_radius->value();
            m_helicoidParams.rotations = ui->doubleSpinBox_helikoida_rotations->value();
            m_helicoidParams.alpha = ui->doubleSpinBox_helikoida_alpha->value();
            m_helicoidParams.radius_segments = ui->spinBox_helikoida_radius_seg->value();
            m_helicoidParams.theta_segments = ui->spinBox_helikoida_theta_seg->value();
            break;
        case BrylaWidget::WstegaMobiusa:
            m_mobiusParams.radius = ui->doubleSpinBox_wstega_radius->value();
            m_mobiusParams.width = ui->doubleSpinBox_wstega_width->value();
            m_mobiusParams.u_segments = ui->spinBox_wstega_u_seg->value();
            m_mobiusParams.v_segments = ui->spinBox_wstega_v_seg->value();
            break;
        default:
            break;
    }
    regenerateShape(currentShape);
}

void MainWindow::regenerateShape(BrylaWidget::Shape shape)
{
    qDebug() << "regenerateShape: shape id =" << shape;
    switch(shape)
    {
        case BrylaWidget::Cylinder:
            ui->brylaWidget->setCurrentShape(shape, m_cylinderParams);
            break;
        case BrylaWidget::Sphere:
            ui->brylaWidget->setCurrentShape(shape, m_sphereParams);
            break;
        case BrylaWidget::Cuboid:
            ui->brylaWidget->setCurrentShape(shape, m_cuboidParams);
            break;
        case BrylaWidget::Cone:
            ui->brylaWidget->setCurrentShape(shape, m_coneParams);
            break;
        case BrylaWidget::Torus:
            ui->brylaWidget->setCurrentShape(shape, m_torusParams);
            break;
        case BrylaWidget::MengerSponge:
             ui->brylaWidget->setCurrentShape(shape, m_mengerParams);
            break;
        case BrylaWidget::Katenoida:
            ui->brylaWidget->setCurrentShape(shape, m_catenoidParams);
            break;
        case BrylaWidget::Helikoida:
            ui->brylaWidget->setCurrentShape(shape, m_helicoidParams);
            break;
        case BrylaWidget::WstegaMobiusa:
            ui->brylaWidget->setCurrentShape(shape, m_mobiusParams);
            break;
        default:
             ui->brylaWidget->setCurrentShape(BrylaWidget::None);
            break;
    }
}
