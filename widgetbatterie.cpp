#include "widgetbatterie.h"
#include <QPainter>

WidgetBatterie::WidgetBatterie(const QString &host, int port, QWidget *parent)
    : QWidget(parent)
    , tcpClientBatterie(host, port)
    , tension(0.0)
{
    setMinimumSize(200, 200);
    connect(&tcpClientBatterie, &TCPclient::newDatas, this, &WidgetBatterie::processTcpDatas);
    connect(&tcpClientBatterie, &TCPclient::connectionChanged, this, &WidgetBatterie::connectionChanged);
}

void WidgetBatterie::processTcpDatas(QString data)
{
    bool ok;
    double v = data.trimmed().toDouble(&ok);
    if (ok) {
        tension = v;
        update();
    }
}

void WidgetBatterie::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    // Corps de la batterie + borne
    QRect r(5, 5, width() - 15, height() - 10);
    p.drawRect(r);
    p.drawRect(r.right(), height() / 2 - 5, 10, 10);
    // Pourcentage de charge (6 V = vide, 8,4 V = pleine)
    double pct = qBound(0.0, (tension - 6.0) / (8.4 - 6.0) * 100.0, 100.0);
    // 5 barres : nombre de barres pleines, marge e, largeur w, hauteur h
    int full = int((pct + 19) / 20);
    int e = 3;
    int w = (r.width() - (6 * e)) / 5;
    int h = r.height() - 2 * e;
    for (int i = 0; i < 5; i++) {
        QRect b(r.left() + e + i * (w + e), r.top() + e, w, h);
        QColor c = (i < full) ? QColor(Qt::green) : QColor(Qt::gray);
        if (pct < 30)
            c = (i < full) ? QColor(255, 165, 0) : QColor(Qt::gray);
        if (pct < 15)
            c = (i < full) ? QColor(Qt::red) : QColor(Qt::gray);
        p.fillRect(b, c);
        p.drawRect(b);
    }

    p.drawText(r, Qt::AlignCenter, QString("%1 V").arg(tension, 0, 'f', 2));
}
