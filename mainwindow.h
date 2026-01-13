#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QVector>
#include "Parameters.h"
#include "brylawidget.h"

class QButtonGroup;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_shape_button_clicked(int id);
    void on_color_button_clicked(int id);
    void on_params_changed();

private:
    void initializeUI();
    void connectSignals();
    void regenerateShape(BrylaWidget::Shape shape);

    Ui::MainWindow *ui;
    QButtonGroup *m_shapeButtonGroup;
    QButtonGroup *m_colorButtonGroup;
    QVector<QColor> m_colors;
    
    CylinderParameters m_cylinderParams;
    SphereParameters m_sphereParams;
    CuboidParameters m_cuboidParams;
    ConeParameters m_coneParams;
    TorusParameters m_torusParams;
    CatenoidParameters m_catenoidParams;
    HelicoidParameters m_helicoidParams;
    MobiusStripParameters m_mobiusParams;
    KostkaMengeraParameters m_kostkaMengeraParams;
};
#endif // MAINWINDOW_H
