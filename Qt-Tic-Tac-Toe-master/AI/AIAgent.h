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
    AIAgent(BoardMarks playerMark, const Board& borad_ref,QObject *parent_ = nullptr)
    : Player(playerMark,borad_ref,parent_) {}
    virtual ~AIAgent() {}
    /**
     * @brief reset function that will reset the state of any search algorithm.
     */
};

#endif // AIAGENT_H
