#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QTimer>

// Client TCP non bloquant avec reconnexion automatique.
// Émet connectionChanged(true/false) à chaque changement d'état de la connexion.
class TCPclient : public QObject
{
    Q_OBJECT
public:
    TCPclient(const QString &_Host, int _port, QObject *parent = nullptr);
    ~TCPclient() override;

    bool sendDatas(const QString &datas);   // false si le socket n'est pas connecté
    bool isConnected() const;

signals:
    void newDatas(QString datas);
    void connectionChanged(bool connected);

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void tryReconnect();

private:
    QString Host;
    int port;
    QTcpSocket clientSocket;
    QTimer reconnectTimer;
};

#endif // TCPCLIENT_H
