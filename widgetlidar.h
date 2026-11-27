#ifndef WIDGETLIDAR_H
#define WIDGETLIDAR_H

#include "tcpclient.h"
#include <QWidget>
#include <array>

class WidgetLidar : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetLidar(const QString &host, int port, QWidget *parent = nullptr);
    bool isConnected() const { return tcplidar.isConnected(); }

signals:
    void connectionChanged(bool connected);

public slots:
    void processTcpDatas(QString data);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::array<int, 360> distancesLidar;   // une distance par degré
    TCPclient tcplidar;
};

#endif // WIDGETLIDAR_H
