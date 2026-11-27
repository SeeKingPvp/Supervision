#ifndef WIDGETVOITURE_H
#define WIDGETVOITURE_H

#include "tcpclient.h"
#include <QWidget>
#include <array>

class WidgetVoiture : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetVoiture(const QString &host, int port, QWidget *parent = nullptr);
    bool isConnected() const { return tcpvoiture.isConnected(); }
    double vitesse;
    double angle;

signals:
    void connectionChanged(bool connected);
    // index : 0 = côté gauche, 1 = milieu gauche, 2 = milieu droite, 3 = côté droite
    void tfStateChanged(int index, bool connected);

public slots:
    // Format reçu : "vitesse;angle" ou "vitesse;angle;G;MG;MD;D" (états TFmini 0/1)
    void processTcpDatas(QString datas);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onConnectionChanged(bool connected);

private:
    void setTfState(int index, bool connected);

    TCPclient tcpvoiture;
    // 0 = côté gauche, 1 = milieu gauche, 2 = milieu droite, 3 = côté droite
    std::array<bool, 4> tfConnected{false, false, false, false};
};

#endif // WIDGETVOITURE_H

