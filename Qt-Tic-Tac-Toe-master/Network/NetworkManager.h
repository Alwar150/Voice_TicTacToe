#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);

    void connectToServer(const QString &ip, quint16 port);
    void sendMessage(const QString &message);

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString &error);
    void messageSent(const QString &message);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onSocketReadyRead();

private:
    QTcpSocket *socket_;
};

#endif // NETWORKMANAGER_H
