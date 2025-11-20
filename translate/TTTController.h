#ifndef TTTCONTROLLER_H
#define TTTCONTROLLER_H
#include "MiniMaxAgent.h"
#include "HumanPlayer.h"
#include "Board.h"
#include "TTTCommonTypes.h"
#include "TicTacToeGame.h"
#include "NetworkManager.h"
#include <QTcpSocket>
#include <QObject>
#include <memory>
#include <vector>
#include <portaudio.h>
#include <pocketsphinx.h>
#include <signal.h>

using std::unique_ptr;
using std::vector;

/**
 * @brief The TTTController class is the controller for a Tic Tac Toe game,
 * it takes in game options as input, and controls the view (TicTacToeGame)
 * and the model (Board) according to the game logic specified in this class,
 * it is also responsible for calling the AI agent to play its turn if specified
 * by the game options.
 */

/**
 * @brief La clase TTTController actúa como el controlador (C de MVC) para un juego de Tic-Tac-Toe.
 *
 * TTTController gestiona la interacción entre la interfaz de usuario (la Vista, TicTacToeGame),
 * el estado del juego (el Modelo, Board) y la lógica de juego, incluyendo la gestión de
 * turnos, agentes de IA (MiniMaxAgent), jugadores humanos y la comunicación de red (NetworkManager).
 * Se encarga de recibir las opciones de juego y orquestar el flujo de la partida.
 */
class TTTController : public QObject
{
    Q_OBJECT
protected:
    /**
     * @brief La vista (View) del juego de Tic-Tac-Toe.
     */
    TicTacToeGame view_;
    /**
     * @brief El modelo (Model) que representa el estado subyacente del tablero.
     */
    Board board_;
    /**
     * @brief Colección de referencias a los elementos visuales (botones de celda) de la vista.
     */
    vector<Cell> cells_;
    /**
     * @brief Colección de valores y banderas que controlan el modo y opciones del juego.
     */
    const TTTOptions& options_;
    /**
     * @brief El marcador (BoardMarks) del jugador al que le toca jugar el turno actual.
     */
    BoardMarks currentPlayer_;
    /**
     * @brief Puntero único al objeto Player que utiliza la marca BoardMarks::X.
     */
    unique_ptr<Player> playerX_;
    /**
     * @brief Puntero único al objeto Player que utiliza la marca BoardMarks::O.
     */
    unique_ptr<Player> playerO_;
    /**
     * @brief Puntero al objeto NetworkManager para propósitos de comunicación de red (multijugador).
     */
    NetworkManager* network_;

private: // Methods
    /**
     * @brief Configura las conexiones de señales y slots entre la vista (GUI) y el controlador.
     *
     * Este método establece las conexiones necesarias para manejar eventos de la GUI, como
     * clics en las celdas o botones de menú.
     */
    void setConnections();
    /**
     * @brief Reinicia todos los componentes internos del juego.
     *
     * Restablece el modelo (Board), la vista (TicTacToeGame) y prepara los jugadores
     * para una nueva partida. Si el agente de IA debe empezar, realiza el primer movimiento.
     */
    void reset();

protected: // Methods
    /**
     * @brief Actualiza el estado del juego después de un movimiento válido.
     *
     * Actualiza el modelo, refresca la vista, verifica condiciones de victoria/empate
     * y cambia al siguiente jugador.
     *
     * @param cell Referencia a la celda que ha sido seleccionada por el jugador.
     */
    void updateGameState(Cell &cell);
    /**
     * @brief Cambia el marcador del jugador actual (de X a O o de O a X).
     */
    void switchPlayer();

public:
    /**
     * @brief Constructor del controlador del juego.
     *
     * Inicializa el modelo, la vista, los jugadores (HumanPlayer, MiniMaxAgent) y
     * establece las conexiones iniciales.
     *
     * @param options La colección de valores y banderas que controlan el juego.
     * @param parent Puntero al objeto QObject padre que es dueño de esta instancia.
     */
    explicit TTTController(const TTTOptions &options, QObject *parent = nullptr);

    /**
     * @brief Inicia la ejecución de la interfaz gráfica y el juego.
     *
     * Muestra la GUI (generalmente llamando a `view_.exec()` o `view_.show()`)
     * y comienza la lógica del juego.
     */
    virtual void startGame();

    /**
     * @brief Devuelve un puntero al QWidget que representa la vista del juego.
     * @return Puntero al objeto QWidget de la vista.
     */
    QWidget* getView() { return &view_; }

signals:
    // --- NET ---
    /**
     * @brief Señal emitida para enviar un mensaje a través de la red (si el modo es multijugador).
     * @param msg El mensaje a enviar (típicamente una representación del movimiento).
     */
    void sendMessage(const QString& msg);

    // --- Players ---
    /**
     * @brief Señal emitida para indicar a los jugadores que inicien su turno.
     *
     * Esta señal se puede conectar al método `play()` de los jugadores para iniciar
     * la lógica de movimiento (e.g., esperar clic humano o calcular movimiento de IA).
     */
    void playTurn();


public slots:
    /**
     * @brief Slot invocado cuando una celda ha sido clickeada por un jugador humano.
     *
     * Recibe la celda clickeada y ejecuta la lógica de `updateGameState` si el movimiento
     * es válido y es el turno de un jugador local.
     *
     * @param cell Referencia a la celda clickeada que el jugador elige.
     */
    void updateGame(Cell &cell);

private slots:
    /**
     * @brief Slot invocado al recibir un mensaje del NetworkManager.
     *
     * Procesa el mensaje recibido (típicamente un movimiento de un jugador remoto)
     * y actualiza el estado del juego.
     *
     * @param msg La cadena de texto recibida a través de la red.
     */
    void onNetworkMessageReceived(const QString& msg);
};

#endif // TTTCONTROLLER_H
