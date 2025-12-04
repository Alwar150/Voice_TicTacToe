#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QTimer>

/**
 * @file NetworkManager.h
 * @brief Definition of the NetworkManager class for TCP communication
 * @author Miguel Fernández Lorenzo
 * @version 1.0
 * @date November 2025
 */

/**
 * @brief TCP communication manager class.
 *
 * This class provides an interface to connect to a TCP server,
 * send and receive messages asynchronously using QTcpSocket.
 *
 * Emits signals to notify connection status, errors and
 * message reception.
 *
 * @startuml
 * class NetworkManager
 * --
 * -socket_: QTcpSocket*
 * -timer_: QTimer*
 * --
 * +NetworkManager(QObject* parent = nullptr)
 * +connectToServer(const QString&, quint16): void
 * +sendMessage(const QString&): void
 * --
 * NetworkManager ..|> QObject
 * @enduml
 */
class NetworkManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor of the NetworkManager class.
     *
     * Initializes the QTcpSocket.
     *
     * @param parent Pointer to the parent object (QObject).
     */
    explicit NetworkManager(QObject *parent = nullptr);

    /**
     * @brief Attempts to establish a connection with a remote server.
     *
     * Starts the non-blocking connection process. The connection result
     * is notified through the connected() or errorOccurred() signals.
     *
     * @param ip The IP address or host name of the server (e.g., "127.0.0.1").
     * @param port The TCP port number of the server.
     */
    void connectToServer(const QString &ip, quint16 port);

    /**
     * @brief Sends a text message to the connected server.
     *
     * The message is converted to bytes and written to the socket.
     *
     * @param message The text message to send.
     */
    void sendMessage(const QString &message);

signals:
    /**
     * @brief Signal emitted when the TCP connection is successfully established.
     */
    void connected();

    /**
     * @brief Signal emitted when the TCP connection is closed (by host, server or error).
     */
    void disconnected();

    /**
     * @brief Signal emitted when an error occurs during connection or transmission.
     * @param error Description of the error.
     */
    void errorOccurred(const QString &error);

    /**
     * @brief Signal emitted after a message has been sent to the server.
     * @param message The text message that was sent.
     */
    void messageSent(const QString &message);

    /**
     * @brief Signal emitted when a complete message has been received from the server.
     * @param message The received text message.
     */
    void messageReceived(const QString &message);

private slots:
    /**
     * @brief Slot invoked when the QTcpSocket connects successfully.
     *
     * Emits the `connected()` signal.
     */
    void onConnected();

    /**
     * @brief Slot invoked when the QTcpSocket disconnects.
     *
     * Emits the `disconnected()` signal.
     */
    void onDisconnected();

    /**
     * @brief Slot invoked when the QTcpSocket reports an error.
     *
     * Maps the socket error to a readable description and emits `errorOccurred()`.
     *
     * @param socketError The socket error code.
     */
    void onError(QAbstractSocket::SocketError socketError);

    /**
     * @brief Slot invoked when there is new data ready to read in the socket.
     *
     * Reads all available data and processes it as a message.
     */
    void onSocketReadyRead();

private:
    QTcpSocket *socket_; ///< Pointer to the QTcpSocket object that manages the TCP connection.
};

#endif //NETWORKMANAGER_H