#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QTimer>

/**
 * @brief Clase gestora de la comunicación de red TCP.
 *
 * Esta clase proporciona una interfaz para conectar a un servidor TCP,
 * enviar y recibir mensajes de forma asíncrona utilizando QTcpSocket.
 *
 * Emite señales para notificar el estado de la conexión, errores y
 * la recepción de mensajes.
 */
class NetworkManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor de la clase NetworkManager.
     *
     * Inicializa el QTcpSocket.
     *
     * @param parent Puntero al objeto padre (QObject).
     */
    explicit NetworkManager(QObject *parent = nullptr);

    /**
     * @brief Intenta establecer una conexión con un servidor remoto.
     *
     * Inicia el proceso de conexión no bloqueante. El resultado de la conexión
     * se notifica a través de las señales connected() o errorOccurred().
     *
     * @param ip La dirección IP o nombre de host del servidor (ej. "127.0.0.1").
     * @param port El número de puerto TCP del servidor.
     */
    void connectToServer(const QString &ip, quint16 port);

    /**
     * @brief Envía un mensaje de texto al servidor conectado.
     *
     * El mensaje se convierte a bytes y se escribe en el socket.
     *
     * @param message El mensaje de texto a enviar.
     */
    void sendMessage(const QString &message);

signals:
    /**
     * @brief Señal emitida cuando la conexión TCP se ha establecido con éxito.
     */
    void connected();

    /**
     * @brief Señal emitida cuando la conexión TCP se ha cerrado (por el host, el servidor o un error).
     */
    void disconnected();

    /**
     * @brief Señal emitida cuando ocurre un error durante la conexión o la transmisión.
     * @param error Descripción del error.
     */
    void errorOccurred(const QString &error);

    /**
     * @brief Señal emitida después de que un mensaje ha sido enviado al servidor.
     * @param message El mensaje de texto que fue enviado.
     */
    void messageSent(const QString &message);

    /**
     * @brief Señal emitida cuando se ha recibido un mensaje completo del servidor.
     * @param message El mensaje de texto recibido.
     */
    void messageReceived(const QString &message);

private slots:
    /**
     * @brief Slot invocado cuando el QTcpSocket se conecta con éxito.
     *
     * Emite la señal `connected()`.
     */
    void onConnected();

    /**
     * @brief Slot invocado cuando el QTcpSocket se desconecta.
     *
     * Emite la señal `disconnected()`.
     */
    void onDisconnected();

    /**
     * @brief Slot invocado cuando el QTcpSocket reporta un error.
     *
     * Mapea el error del socket a una descripción legible y emite `errorOccurred()`.
     *
     * @param socketError El código de error del socket.
     */
    void onError(QAbstractSocket::SocketError socketError);

    /**
     * @brief Slot invocado cuando hay nuevos datos listos para leer en el socket.
     *
     * Lee todos los datos disponibles y los procesa como un mensaje.
     */
    void onSocketReadyRead();

private:
    QTcpSocket *socket_; ///< Puntero al objeto QTcpSocket que gestiona la conexión TCP.
};

#endif //NETWORKMANAGER_H
