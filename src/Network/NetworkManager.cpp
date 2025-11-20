#include "NetworkManager.h"
#include "TTTController.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent), socket_(new QTcpSocket(this))
{
    connect(socket_, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(socket_, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(socket_, &QTcpSocket::errorOccurred, this, &NetworkManager::onError);
    connect(socket_, &QTcpSocket::readyRead, this, &NetworkManager::onSocketReadyRead);
}

void NetworkManager::connectToServer(const QString &ip, quint16 port)
{
    qDebug() << " [NET] Intentando conectar a" << ip << ":" << port;
    socket_->connectToHost(ip, port);
}

void NetworkManager::sendMessage(const QString &message)
{
    if (socket_->state() == QAbstractSocket::ConnectedState) {
        socket_->write(message.toUtf8() + '\n');
        socket_->flush();
        emit messageSent(message);
    } else {
        emit errorOccurred("[NET] Socket no conectado");
    }
}

void NetworkManager::onConnected()
{
    qDebug() << "[NET] Conectado al servidor.";

}

void NetworkManager::onDisconnected()
{
    qDebug() << "[NET] Desconectado del servidor.";

}

void NetworkManager::onError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    qDebug() << (socket_->errorString());
}

void NetworkManager::onSocketReadyRead()
{
    static QByteArray buffer;

    buffer += socket_->readAll();

    int index;
    while ((index = buffer.indexOf('\n')) != -1) {
        QByteArray line = buffer.left(index);
        buffer.remove(0, index + 1);

        QString msg = QString::fromUtf8(line).trimmed();
        if (!msg.isEmpty()) {
            qDebug() << "[NET] Recibido:" << msg;
            emit messageReceived(msg);
        }
    }
}

