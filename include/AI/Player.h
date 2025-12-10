#ifndef PLAYER_H
#define PLAYER_H
#include <QObject>
#include <TTTCommonTypes.h>
#include <Board.h>

/**
 * @file Player.h
 * @brief Definition of the Player abstract base class for different player types
 * @author Miguel Fernández Lorenzo
 * @version 1.0
 * @date November 2025
 */

/**
 * @class Player
 * @brief The Player class is an abstract base class for different types of players.
 *
 * This class defines the common interface for all participants in the Tic-Tac-Toe game,
 * whether they are human players (HumanPlayer), AI agents (MiniMaxAgent), or network players.
 * It provides the mechanism to start the turn (`play()`) and notify the completed move.
 *
 * @startuml
 * interface Player {
 *   +play(): void
 *   --
 *   -board_: Board*
 *   -mark_: BoardMarks&
 *   --
 *   +playerFinished(int): signal
 * }
 *
 * Player ..|> QObject
 * @enduml
 */

 

class Player : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for the base Player class.
     *
     * Initializes the player with their marker (X or O) and a reference to the game board
     * so the player can query its state.
     *
     * @param playerMark The marker (BoardMarks::X or BoardMarks::O) assigned to the player.
     * @param board_ref Constant reference to the game Board object (Model).
     * @param parent_ Pointer to the parent QObject (nullptr by default).
     */
    explicit Player(const BoardMarks& playerMark, const Board& board_ref, QObject *parent_ = nullptr) :
        QObject(parent_),
        board_(&board_ref),
        mark_(playerMark){}

    /**
     * @brief Virtual destructor.
     *
     * Allows proper destruction of derived classes through a base class pointer.
     */
    virtual ~Player() {}

    /**
     * @brief Pure abstract method to start the player's move logic.
     *
     * Derived classes must implement this method to execute their turn logic
     * (e.g., wait for voice/click input, calculate the best AI move).
     */
    virtual void play() = 0;

signals:
    /**
     * @brief Signal emitted when the player has completed their move.
     *
     * This signal should be emitted by the player after determining their move
     * to notify the controller which cell should be marked.
     *
     * @param cell The index of the cell (0-8) chosen by the player.
     */
    void playerFinished(int const& cell);

protected:
    /**
     * @brief Constant pointer to the game board.
     *
     * Used to query the current state of cells before making a move.
     */
    const Board* board_;

    /**
     * @brief Constant reference to the marker assigned to this player (X or O).
     */
    const BoardMarks mark_;

};

#endif // PLAYER_H
