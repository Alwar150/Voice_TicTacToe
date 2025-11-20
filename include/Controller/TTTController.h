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
 * @file TTTController.h
 * @brief Definition of the TTTController class for the Tic-Tac-Toe game controller (MVC pattern)
 * @author Miguel Fernández Lorenzo
 * @version 1.0
 * @date November 2025
 */

/**
 * @brief The TTTController class acts as the controller (C in MVC) for a Tic-Tac-Toe game.
 *
 * TTTController manages the interaction between the user interface (the View, TicTacToeGame),
 * the game state (the Model, Board) and the game logic, including turn management,
 * AI agents (MiniMaxAgent), human players and network communication (NetworkManager).
 * It is responsible for receiving game options and orchestrating the game flow.
 */
class TTTController : public QObject
{
    Q_OBJECT
protected:
    /**
     * @brief The view (View) of the Tic-Tac-Toe game.
     */
    TicTacToeGame view_;
    /**
     * @brief The model (Model) representing the underlying board state.
     */
    Board board_;
    /**
     * @brief Collection of references to the visual elements (cell buttons) of the view.
     */
    vector<Cell> cells_;
    /**
     * @brief Collection of values and flags that control the game mode and options.
     */
    const TTTOptions& options_;
    /**
     * @brief The marker (BoardMarks) of the player whose turn it is to play.
     */
    BoardMarks currentPlayer_;
    /**
     * @brief Unique pointer to the Player object using the BoardMarks::X marker.
     */
    unique_ptr<Player> playerX_;
    /**
     * @brief Unique pointer to the Player object using the BoardMarks::O marker.
     */
    unique_ptr<Player> playerO_;
    /**
     * @brief Pointer to the NetworkManager object for network communication purposes (multiplayer).
     */
    NetworkManager* network_;

private: // Methods
    /**
     * @brief Configures signal and slot connections between the view (GUI) and the controller.
     *
     * This method establishes the necessary connections to handle GUI events, such as
     * clicks on cells or menu buttons.
     */
    void setConnections();
    /**
     * @brief Resets all internal game components.
     *
     * Resets the model (Board), the view (TicTacToeGame) and prepares the players
     * for a new game. If the AI agent should start, it makes the first move.
     */
    void reset();

protected: // Methods
    /**
     * @brief Updates the game state after a valid move.
     *
     * Updates the model, refreshes the view, checks victory/tie conditions
     * and switches to the next player.
     *
     * @param cell Reference to the cell that has been selected by the player.
     */
    void updateGameState(Cell &cell);
    /**
     * @brief Switches the current player marker (from X to O or from O to X).
     */
    void switchPlayer();

public:
    /**
     * @brief Constructor of the game controller.
     *
     * Initializes the model, view, players (HumanPlayer, MiniMaxAgent) and
     * establishes the initial connections.
     *
     * @param options The collection of values and flags that control the game.
     * @param parent Pointer to the parent QObject that owns this instance.
     */
    explicit TTTController(const TTTOptions &options, QObject *parent = nullptr);

    /**
     * @brief Starts the graphical interface execution and the game.
     *
     * Shows the GUI (usually by calling `view_.exec()` or `view_.show()`)
     * and begins the game logic.
     */
    virtual void startGame();

    /**
     * @brief Returns a pointer to the QWidget representing the game view.
     * @return Pointer to the view's QWidget object.
     */
    QWidget* getView() { return &view_; }

signals:
    // --- NET ---
    /**
     * @brief Signal emitted to send a message through the network (if the mode is multiplayer).
     * @param msg The message to send (typically a representation of the move).
     */
    void sendMessage(const QString& msg);

    // --- Players ---
    /**
     * @brief Signal emitted to indicate to players to start their turn.
     *
     * This signal can be connected to the `play()` method of players to start
     * the move logic (e.g., wait for human click or calculate AI move).
     */
    void playTurn();

public slots:
    /**
     * @brief Slot invoked when a cell has been clicked by a human player.
     *
     * Receives the clicked cell and executes the `updateGameState` logic if the move
     * is valid and it's a local player's turn.
     *
     * @param cell Reference to the clicked cell that the player chooses.
     */
    void updateGame(Cell &cell);

private slots:
    /**
     * @brief Slot invoked when receiving a message from the NetworkManager.
     *
     * Processes the received message (typically a move from a remote player)
     * and updates the game state.
     *
     * @param msg The text string received through the network.
     */
    void onNetworkMessageReceived(const QString& msg);
};

#endif // TTTCONTROLLER_H