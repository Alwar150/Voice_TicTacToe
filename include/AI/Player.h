#ifndef PLAYER_H
#define PLAYER_H
// ... includes ...

/**
 * @brief La clase Player es una clase base abstracta para diferentes tipos de jugadores.
 *
 * Esta clase define la interfaz común para todos los participantes del juego
 * de Tic-Tac-Toe, ya sean jugadores humanos (HumanPlayer), agentes de IA
 * (MiniMaxAgent) o jugadores de red. Proporciona el mecanismo para iniciar
 * el turno (`play()`) y notificar el movimiento realizado.
 */
class Player : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor de la clase base Player.
     *
     * Inicializa el jugador con su marcador (X u O) y una referencia al tablero
     * del juego para que el jugador pueda consultar su estado.
     *
     * @param playerMark El marcador (BoardMarks::X o BoardMarks::O) asignado al jugador.
     * @param board_ref Referencia constante al objeto Board (Modelo) del juego.
     * @param parent_ Puntero al objeto QObject padre (nullptr por defecto).
     */
    explicit Player(const BoardMarks& playerMark, const Board& board_ref, QObject *parent_ = nullptr) :
        QObject(parent_),
        board_(&board_ref),
        mark_(playerMark){}

    /**
     * @brief Destructor virtual.
     *
     * Permite la correcta destrucción de las clases derivadas a través de un puntero a la clase base.
     */
    virtual ~Player() {}

    /**
     * @brief Método abstracto puro para iniciar la lógica de movimiento del jugador.
     *
     * Las clases derivadas deben implementar este método para ejecutar la lógica de su turno
     * (e.g., esperar una entrada de voz/clic, calcular el mejor movimiento de IA).
     */
    virtual void play() = 0;

signals:
    /**
     * @brief Señal emitida cuando el jugador ha completado su movimiento.
     *
     * Esta señal debe ser emitida por el jugador después de determinar su movimiento
     * para notificar al controlador qué celda debe ser marcada.
     *
     * @param cell El índice de la celda (0-8) elegida por el jugador.
     */
    void playerFinished(int const& cell);

protected:
    /**
     * @brief Puntero constante al tablero del juego.
     *
     * Usado para consultar el estado actual de las celdas antes de realizar un movimiento.
     */
    const Board* board_;

    /**
     * @brief Referencia constante al marcador asignado a este jugador (X o O).
     */
    const BoardMarks& mark_;

};

#endif // PLAYER_H
