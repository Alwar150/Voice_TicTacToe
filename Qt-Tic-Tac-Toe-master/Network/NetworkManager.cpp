#include "NetworkManager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent), socket_(new QTcpSocket(this))
{
    connect(socket_, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(socket_, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(socket_, &QTcpSocket::errorOccurred, this, &NetworkManager::onError);
}

void NetworkManager::connectToServer(const QString &ip, quint16 port)
{
    qDebug() << "Intentando conectar a" << ip << ":" << port;
    socket_->connectToHost(ip, port);
}

void NetworkManager::sendMessage(const QString &message)
{
    if (socket_->state() == QAbstractSocket::ConnectedState) {
        socket_->write(message.toUtf8() + '\n');
        socket_->flush();
        emit messageSent(message);
    } else {
        emit errorOccurred("Socket no conectado");
    }
}

void NetworkManager::onConnected()
{
    qDebug() << "Conectado al servidor.";
    emit connected();
}

void NetworkManager::onDisconnected()
{
    qDebug() << "Desconectado del servidor.";
    emit disconnected();
}

void NetworkManager::onError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    emit errorOccurred(socket_->errorString());
}
