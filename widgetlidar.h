#ifndef WIDGETLIDAR_H
#define WIDGETLIDAR_H

#include "tcpclient.h"
#include <QWidget>
#include <array>

class WidgetLidar : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetLidar(const QString &host, int port,QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void processTcpDatas(QString data);
private:
    std::array<int, 360> distancesLidar;   // une distance par degré
    TCPclient tcplidar;
};

#endif
