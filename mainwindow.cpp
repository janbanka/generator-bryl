#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myglwidget.h"
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_shapeButtonGroup = new QButtonGroup(this);
    m_shapeButtonGroup->addButton(ui->radioButtonWalec, MyGLWidget::Cylinder);
    m_shapeButtonGroup->addButton(ui->radioButtonKula, MyGLWidget::Sphere);
    m_shapeButtonGroup->addButton(ui->radioButtonProstopadloscian, MyGLWidget::Cuboid);
    m_shapeButtonGroup->addButton(ui->radioButtonStozek, MyGLWidget::Cone);

    connect(m_shapeButtonGroup, SIGNAL(idClicked(int)), this, SLOT(on_shape_button_clicked(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_shape_button_clicked(int id)
{
    ui->openGLWidget->setCurrentShape(id);
}
