#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H
#include <Player.h>
#include <SpeechManager.h>
#include <QThread>

/**
 * @brief La clase HumanPlayer representa al jugador humano en el juego.
 *
 * Hereda de la clase base abstracta Player. Se encarga de capturar la entrada
 * del usuario (ya sea por clic en la GUI o, en este caso, por reconocimiento
 * de voz mediante SpeechManager) y convertirla en un movimiento válido en el tablero.
 *
 * Utiliza un hilo de trabajo (`QThread`) dedicado para el procesamiento de voz
 * (`SpeechManager`) para evitar bloquear el hilo principal de la interfaz gráfica.
 */
class HumanPlayer final : public Player{
    Q_OBJECT

public:
    /**
     * @brief Constructor del jugador humano.
     *
     * Inicializa la referencia al tablero y el marcador del jugador. También
     * crea e inicializa el objeto SpeechManager y el hilo asociado.
     *
     * @param board_ref Referencia constante al modelo del tablero.
     * @param playerMark El marcador (BoardMarks::X o BoardMarks::O) asignado al jugador.
     * @param parent Puntero al objeto QObject padre.
     */
    explicit HumanPlayer(const Board& board_ref, BoardMarks playerMark = BoardMarks::Empty, QObject* parent = nullptr);

    /**
     * @brief Destructor del jugador humano.
     *
     * Se encarga de limpiar y terminar de forma segura el hilo de reconocimiento de voz.
     */
    virtual ~HumanPlayer() {}

    /**
     * @brief Inicia el turno del jugador humano.
     *
     * Este método sobrescrito inicia el hilo de reconocimiento de voz (`SpeechManager`)
     * y le indica que comience la escucha, esperando un comando del usuario.
     */
    void play() override;

signals:
    /**
     * @brief Señal emitida para iniciar la escucha del micrófono en el hilo de trabajo.
     */
    void startListening();

    /**
     * @brief Señal emitida para detener la escucha del micrófono y apagar el hilo de trabajo.
     */
    void stopListening();

private:
    /**
     * @brief Procesa el texto reconocido por el SpeechManager.
     *
     * Este método recibe la hipótesis de texto (`text`) del sistema de reconocimiento de voz
     * e inicia la validación del comando.
     *
     * @param text La cadena de texto reconocida (comando de voz).
     */
    void parseSpeechCommand(const QString &text);

    /**
     * @brief Valida si el comando de voz reconocido corresponde a un movimiento válido.
     *
     * Verifica que el comando sea entendible (ej. "jugar celda 5") y que la celda
     * indicada esté disponible en el tablero.
     *
     * @param command La cadena de texto a validar.
     * @return true si el comando es válido y la celda está disponible.
     */
    bool validateCommand(const QString &command);

    /**
     * @brief Slot encargado de gestionar el ciclo de vida del hilo de trabajo.
     *
     * Ejecuta el proceso de finalización segura del hilo (`quit()`, `wait()`)
     * y la liberación de recursos (`QThread::deleteLater`).
     */
    void handleThreadCleanup();

    SpeechManager* stt_;    ///< Puntero al objeto SpeechManager, el *worker* para el reconocimiento de voz.
    QThread* sttThread_;    ///< Puntero al hilo de trabajo dedicado a ejecutar el SpeechManager.
};

#endif // HUMANPLAYER_H
