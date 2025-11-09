#ifndef TTTCONTROLLER_H
#define TTTCONTROLLER_H
#include "AIAgent.h"
#include "Board.h"
#include "TTTCommonTypes.h"
#include "TicTacToeGame.h"
#include "NetworkManger.h"
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
     * @brief agent is the AI agent chosen to play against the human player.
     */
    unique_ptr<AIAgent> agent_;

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

    QTcpSocket *socket_;
    void setupNetwork();
    void sendMove(int row, int col, char player);

protected: // Methods
    /**
     * @brief updateGameState updates the view and the model and switches the current player.
     * @param cell: the clicked cell that the player chooses.
     */
    virtual void updateGameState(Cell &cell);
    /**
     * @brief AIAgentPlay calls the AI agent to play given the current board inputs.
     */
    virtual void AIAgentPlay();
    /**
     * @brief HumanPlay calls the Speech Recognition Engine for decoding input into Cell.
     */
    virtual void HumanPlay();
    /**
     * @brief switchPlayer switches the current player mark (X to O and O to X).
     */
    virtual void switchPlayer();

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
    int parseSpeechToCell(const QString &text)
    {
        QStringList keywords = text.split(' ', Qt::SkipEmptyParts);
        if (keywords.size() < 2) return defaults::INVALID_CELL;

        QString k1 = keywords[0];
        QString k2 = keywords[1];

        if (k1 == "arriba") {
            if (k2 == "izquierda") return 0;
            if (k2 == "arriba") return 1;
            if (k2 == "derecha") return 2;
        } else if (k1 == "centro") {
            if (k2 == "izquierda") return 3;
            if (k2 == "centro") return 4;
            if (k2 == "derecha") return 5;
            if (k2 == "arriba") return 1;
            if (k2 == "abajo") return 7;
        } else if (k1 == "abajo") {
            if (k2 == "izquierda") return 6;
            if (k2 == "centro") return 7;
            if (k2 == "derecha") return 8;
        }
        return defaults::INVALID_CELL;
    }
    QWidget* getView() { return &view_; }
signals:
    /**
     * @brief turnFinished is emitted when the human player is finished with the cell input,
     * and the internal game state is supposed to be updated.
     */
    void AIFinished();

    void humanFinished();
    void speechRecognized(QString);


public slots:
    /**
     * @brief updateGame sets the cell clicked in the model (board).
     * @param cell: the clicked cell that the player chooses.
     */
    virtual void updateGame(Cell &cell);
private slots:
    void onSpeechRecognized(const QString &text);

};

#endif // TTTCONTROLLER_H
