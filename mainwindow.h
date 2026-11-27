#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpclient.h"
#include "widgetbatterie.h"
#include "widgetlidar.h"
#include "widgetvoiture.h"
#include <QMainWindow>
#include <array>

class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void start();
    void stop();

private:
    // Met à jour un label d'état : "Nom : connecté" (vert) ou "Nom : déconnecté" (rouge)
    void setStatus(QLabel *label, const QString &nom, bool connecte, bool feminin = false);

    Ui::MainWindow *ui;
    TCPclient startS;           // commandes "start" / "stop"
    WidgetLidar lidar360;
    WidgetVoiture voiture;
    WidgetBatterie batterie;
    std::array<QLabel *, 4> tfLabels;   // TFmini15 à TFmini18
};
#endif // MAINWINDOW_H
