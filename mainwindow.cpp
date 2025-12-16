#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "brylawidget.h"
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Shape selection setup
    m_shapeButtonGroup = new QButtonGroup(this);
    m_shapeButtonGroup->addButton(ui->radioButtonWalec, BrylaWidget::Cylinder);
    m_shapeButtonGroup->addButton(ui->radioButtonKula, BrylaWidget::Sphere);
    m_shapeButtonGroup->addButton(ui->radioButtonProstopadloscian, BrylaWidget::Cuboid);
    m_shapeButtonGroup->addButton(ui->radioButtonStozek, BrylaWidget::Cone);
    connect(m_shapeButtonGroup, SIGNAL(idClicked(int)), this, SLOT(on_shape_button_clicked(int)));

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
    connect(m_colorButtonGroup, SIGNAL(idClicked(int)), this, SLOT(on_color_button_clicked(int)));

    // Set default color
    ui->colorButton3->setChecked(true); // Blue default
    on_color_button_clicked(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_shape_button_clicked(int id)
{
    ui->brylaWidget->setCurrentShape(static_cast<BrylaWidget::Shape>(id));
    int colorId = m_colorButtonGroup->checkedId();
    if(colorId != -1) {
        on_color_button_clicked(colorId);
    }
}

void MainWindow::on_color_button_clicked(int id)
{
    if(id >= 0 && id < m_colors.size()) {
        ui->brylaWidget->setShapeColor(m_colors[id]);
    }
}
