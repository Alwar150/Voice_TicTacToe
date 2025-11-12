#ifndef AIAGENT_H
#define AIAGENT_H
#include "Player.h"
/**
 * @brief The AIAgent Interface describes
 * the behaviour of any implemented AI agents.
 */

class Board;
class AIAgent : public Player
{
public:
    AIAgent(QObject *parent_ = nullptr, BoardMarks playerMark, const Board& borad_ref)
    : Player(parent_,playerMark,borad_ref) {}
    virtual ~AIAgent() {}
    /**
     * @brief Start the algorithm to choose a cell to play based on a given board.
     * @param board: a reference to the board model.
     * @return the chosen cell index (in a 1D array) that the AI played.
     */
    virtual int play(const Board &board) = 0;
    /**
     * @brief reset function that will reset the state of any search algorithm.
     */
    virtual void reset() {}
};

#endif // AIAGENT_H
