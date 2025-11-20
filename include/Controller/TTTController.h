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

class TTTController : public QObject
{
    Q_OBJECT
protected:
    /**
     * @brief view is the view class of a tic tac toe game.
     */
    TicTacToeGame view_;
    /**
     * @brief board is the model that represents the underlying layer of a game.
     */
    Board board_;
    /**
     * @brief cells are the collection of cell buttons built by the view class.
     */
    vector<Cell> cells_;
    /**
     * @brief options is the collection of values and flags that controls the game.
     */
    const TTTOptions& options_;
    /**
     * @brief currentPlayer is the player that will play the current turn.
     */
    BoardMarks currentPlayer_;
    /**
     * @brief player holding the Boardmarks::X
     */
    unique_ptr<Player> playerX_;
    /**
     * @brief player holding the Boardmarks::X
     */
    unique_ptr<Player> playerO_;
    /**
     * @brief network managment object for socket communication purposes
     */
    NetworkManager* network_;

private: // Methods
    /**
     * @brief setConnections will set a connection to all GUI elements to slots defined
     * to handle these events.
     */
    void setConnections();
    /**
     * @brief resets all internal components, the view, the model and the AI agent.
     * then if AI is supposed to start a game it will play first.
     */
    void reset();

protected: // Methods
    /**
     * @brief updateGameState updates the view and the model and switches the current player.
     * @param cell: the clicked cell that the player chooses.
     */
    void updateGameState(Cell &cell);
    /**
     * @brief switchPlayer switches the current player mark (X to O and O to X).
     */
    void switchPlayer();

public:
    /**
     * @brief TTTController constructs a game controller.
     * @param options is the collection of values and flags that controls the game.
     * @param parent is a QObject type parent that owns this instance of the controller.
     */
    explicit TTTController(const TTTOptions &options, QObject *parent = nullptr);
    /**
     * @brief startGame executes this QDialog to show the GUI and start the gameplay.
     */
    virtual void startGame();

    QWidget* getView() { return &view_; }

signals:
    // NET

    // Players




public slots:
    /**
     * @brief updateGame sets the cell clicked in the model (board).
     * @param cell: the clicked cell that the player chooses.
     */
    void updateGame(Cell &cell);

private slots:
    void onNetworkMessageReceived(const QString& msg);
};

#endif // TTTCONTROLLER_H
