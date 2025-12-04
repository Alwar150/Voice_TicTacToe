#ifndef MINIMAXAGENT_H
#define MINIMAXAGENT_H
#include "AIAgent.h"

/**
 * @file MiniMaxAgent.h
 * @brief Definition of the MiniMaxAgent class implementing the Minimax algorithm with Alpha-Beta pruning
 * @author Miguel Fernández Lorenzo
 * @version 1.0
 * @date November 2025
 */

/**
 * @brief The MiniMaxAgent class implements the logic to select the best move using the Minimax algorithm.
 *
 * Inherits from AIAgent (the base class for AI agents). This agent recursively simulates
 * all possible future moves until reaching a final state or the maximum search depth (`depth_`).
 * Uses **Alpha-Beta pruning** to optimize the search.
 * The algorithm evaluates final states by assigning scores: positive for AI victory,
 * negative for human player victory, and zero for a tie.
 *
 * @startuml
 * class MiniMaxAgent extends AIAgent
 * --
 * -depth_: unsigned short
 * --
 * +MiniMaxAgent(const Board&, BoardMarks, unsigned short, QObject*)
 * +play(): void
 * -maxMove(Board&, unsigned short, short, short): short
 * -minMove(Board&, unsigned short, short, short): short
 * -score(const BoardState): short
 * +<<signal>>  playerFinished(int)
 * --
 * Player ..|> QObject
 * AIAgent ..|> Player
 * MiniMaxAgent ..|> AIAgent
 * @enduml
 */

class MiniMaxAgent final : public AIAgent
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the MiniMaxAgent class.
     *
     * Initializes the agent with the board, the marker assigned to the AI, and the maximum
     * depth for the Minimax search.
     *
     * @param board Constant reference to the game board.
     * @param AImark The marker (BoardMarks::X or BoardMarks::O) assigned to the AI.
     * @param depth The maximum search depth (cutoff) for the Minimax algorithm.
     * @param parent Pointer to the parent QObject.
     */
    explicit MiniMaxAgent(const Board& board, BoardMarks AImark, unsigned short depth, QObject* parent = nullptr);

    /**
     * @brief Starts the Minimax algorithm to determine and execute the AI's best move.
     *
     * This method is the implementation of the abstract function from the `Player` class.
     * Calculates the best cell and emits the `playerFinished` signal with the index of the chosen cell.
     */
    void play() override;

private:
    /**
     * @brief Maximum search depth of the Minimax algorithm tree.
     *
     * Defines the limit of how many future moves the agent will simulate.
     */
    const unsigned short depth_;

    /**
     * @brief Returns the human player's marker.
     *
     * Automatically determines the opponent player's marker based on the AI's marker.
     * @return The marker (BoardMarks::X or BoardMarks::O) of the human player.
     */
    inline BoardMarks playerMark_() const {
        return (BoardMarks::X == mark_) ? BoardMarks::O : BoardMarks::X;
    }

    /**
     * @brief Score assigned to the final state when the AI wins the game.
     */
    static constexpr short AI_WIN_SCORE = 1;

    /**
     * @brief Score assigned to the final state when the human player wins the game.
     */
    static constexpr short PLAYER_WIN_SCORE = -1;

    /**
     * @brief Score assigned to the final state when the game ends in a tie.
     */
    static constexpr short TIE_SCORE = 0;

    /**
     * @brief Maximizer node function (AI's turn).
     *
     * Simulates the AI's turn, choosing the move that maximizes the final score
     * and applying Alpha-Beta pruning.
     *
     * @param board Reference to the board model (temporarily modified during simulation).
     * @param depth The current depth of the search tree.
     * @param alpha The minimum value guaranteed for the maximizer (AI).
     * @param beta The maximum value guaranteed for the minimizer (Player).
     * @return The maximum possible score for the current state.
     */
    short maxMove(Board &board, unsigned short depth, short alpha, short beta) const;

    /**
     * @brief Minimizer node function (human player's turn).
     *
     * Simulates the human player's turn, choosing the move that minimizes the AI's final score
     * and applying Alpha-Beta pruning.
     *
     * @param board Reference to the board model (temporarily modified during simulation).
     * @param depth The current depth of the search tree.
     * @param alpha The minimum value guaranteed for the maximizer (AI).
     * @param beta The maximum value guaranteed for the minimizer (Player).
     * @return The minimum possible score for the current state.
     */
    short minMove(Board &board, unsigned short depth, short alpha, short beta) const;

    /**
     * @brief Assigns a score to a final game state.
     *
     * Evaluates the board state by determining if there is a winner, a loser, or a tie.
     *
     * @param state The final game state (BoardState).
     * @return A score based on the game state (AI_WIN_SCORE, PLAYER_WIN_SCORE, TIE_SCORE).
     */
    short score(const BoardState state) const;
};

#endif // MINIMAXAGENT_H