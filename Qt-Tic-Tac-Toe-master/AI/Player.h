#ifndef PLAYER_H
#define PLAYER_H    
#include <QObject>
#include "Board.h"
#include "TTTCommonTypes.h"
/**
 * @brief The Player class is an abstract base class for different types of players
 * in the Tic Tac Toe game, such as human players and AI players.
 */
class Player : public QObject
{
    Q_OBJECT
    public:
    explicit Player(const BoardMarks& playerMark, const Board& board_ref, QObject *parent_ = nullptr) :
        QObject(parent_),
        board_(&board_ref),
        mark_(playerMark){}
    virtual ~Player() {}
    virtual void play() = 0;

    signals:
    void playerFinished(int const& cell);

    protected:
    
    const Board* board_;
    const BoardMarks& mark_;

};

#endif // PLAYER_H
