#ifndef AIAGENT_H
#define AIAGENT_H
#include "Player.h"

/**
 * @file AIAgent.h
 * @brief Definition of the AIAgent interface for artificial intelligence agents
 * @author Miguel Fernández Lorenzo
 * @version 1.0
 * @date November 2025
 */

/**
 * @brief The AIAgent interface describes the behavior of any implemented Artificial Intelligence agent.
 *
 * This abstract base class inherits from Player and establishes a common interface
 * for all AI agents (such as MiniMaxAgent). It defines the basic functions
 * necessary for an agent to reset its internal state before
 * a new turn or game.
 */

class Board; // Forward declaration

class AIAgent : public Player
{
public:
    /**
     * @brief Constructor for the AIAgent base class.
     *
     * Initializes the agent with its marker and a reference to the board.
     *
     * @param playerMark The marker (BoardMarks::X or BoardMarks::O) assigned to the AI.
     * @param borad_ref Constant reference to the game Board object (Model).
     * @param parent_ Pointer to the parent QObject (nullptr by default).
     */
    AIAgent(BoardMarks playerMark, const Board& borad_ref, QObject *parent_ = nullptr)
        : Player(playerMark, borad_ref, parent_) {}

    /**
     * @brief Virtual destructor.
     *
     * Allows proper destruction of derived classes through a base class pointer.
     */
    virtual ~AIAgent() {}

    /**
     * @brief Reset function that restores the state of any internal search algorithm.
     *
     * This method is typically called at the beginning of a new game.
     * Derived classes must implement this method to clear any cache memory
     * or data structures used by their algorithms (although it's not a pure abstract method here,
     * the function's intent is defined).
     */
    virtual void reset() = 0; // Made abstract to force implementation in derived classes
};

#endif // AIAGENT_H