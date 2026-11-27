#include "tcpclient.h"
#include <QDataStream>
#include <QDebug>

TCPclient::TCPclient(const QString &_Host, int _port, QObject *parent)
    : QObject(parent)
    , Host(_Host)
    , port(_port)
{
    // Important : il faut lier les signaux du socket aux slots !
    connect(&clientSocket, &QTcpSocket::readyRead, this, &TCPclient::onReadyRead);
    connect(&clientSocket, &QTcpSocket::connected, this, &TCPclient::onConnected);
    connect(&clientSocket, &QTcpSocket::disconnected, this, &TCPclient::onDisconnected);

    // Reconnexion automatique toutes les 2 s tant que le socket n'est pas connecté
    // (non bloquant : l'interface reste réactive)
    connect(&reconnectTimer, &QTimer::timeout, this, &TCPclient::tryReconnect);
    reconnectTimer.start(2000);
    tryReconnect();
}

TCPclient::~TCPclient()
{
    // Évite d'émettre des signaux vers des objets déjà détruits à la fermeture
    reconnectTimer.stop();
    QObject::disconnect(&clientSocket, nullptr, this, nullptr);
}

bool TCPclient::isConnected() const
{
    return clientSocket.state() == QAbstractSocket::ConnectedState;
}

void TCPclient::tryReconnect()
{
    if (clientSocket.state() == QAbstractSocket::UnconnectedState) {
        qDebug() << "Tentative de connexion vers" << Host << ":" << port << "...";
        clientSocket.connectToHost(Host, static_cast<quint16>(port));
    }
}

void TCPclient::onConnected()
{
    qDebug() << "Connexion réussie vers" << Host << ":" << port;
    emit connectionChanged(true);
}

void TCPclient::onDisconnected()
{
    qDebug() << "Déconnecté de" << Host << ":" << port;
    emit connectionChanged(false);
}

bool TCPclient::sendDatas(const QString &datas)
{
    if (!isConnected()) {
        qDebug() << "Impossible d'envoyer : le socket n'est pas connecté.";
        return false;
    }

    QByteArray block;
    QDataStream dataOut(&block, QIODevice::WriteOnly);
    dataOut.setVersion(QDataStream::Qt_5_0); // Doit correspondre à la version de onReadyRead

    // Qt encapsule la taille de la chaîne devant les données
    dataOut << datas;

    // Une fois le message encapsulé, il est envoyé via le socket vers le serveur
    clientSocket.write(block);
    qDebug() << "Client :" << datas;
    return true;
}

void TCPclient::onReadyRead()
{
    QDataStream in(&clientSocket);
    in.setVersion(QDataStream::Qt_5_0); // Doit correspondre à la version de sendDatas

    // On boucle pour traiter tous les messages si plusieurs paquets sont arrivés ensemble
    while (true) {
        in.startTransaction();

        QString messageRecu;
        in >> messageRecu; // Tente d'extraire le QString complet

        if (in.commitTransaction()) {
            // La transaction a réussi, le message est complet
            emit newDatas(messageRecu);
        } else {
            // Message incomplet : on sort et on attend le prochain readyRead
            break;
        }
    }
}
