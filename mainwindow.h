#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpclient.h"
#include "widgetbatterie.h"
#include "widgetlidar.h"
#include "widgetvoiture.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //#define ip = "10.98.32.173"
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void start();
    void stop();
private:
    Ui::MainWindow *ui;
    TCPclient startS;
    WidgetLidar lidar360;
    WidgetVoiture voiture;
    WidgetBatterie batterie;
};
#endif // MAINWINDOW_H
