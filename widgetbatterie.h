#ifndef WIDGETBATTERIE_H
#define WIDGETBATTERIE_H

#include "tcpclient.h"
#include <QWidget>

class WidgetBatterie : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetBatterie(const QString &host, int port, QWidget *parent = nullptr);
    bool isConnected() const { return tcpClientBatterie.isConnected(); }

signals:
    void connectionChanged(bool connected);

public slots:
    void processTcpDatas(QString data);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    TCPclient tcpClientBatterie;
    double tension;
};

#endif // WIDGETBATTERIE_H
