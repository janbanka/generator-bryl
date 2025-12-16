#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QVector>

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


private:
    Ui::MainWindow *ui;
    QButtonGroup *m_shapeButtonGroup;
    QButtonGroup *m_colorButtonGroup;
    QVector<QColor> m_colors;
};
#endif // MAINWINDOW_H
