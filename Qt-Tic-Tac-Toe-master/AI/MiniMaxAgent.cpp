#include "MiniMaxAgent.h"
#include <QPair>
#include <limits.h>

MiniMaxAgent::MiniMaxAgent(QObject* parent = nullptr, BoardMarks AImark, const Board& board,unsigned short depth)
    : AIAgent(parent,AImark,board), depth_(depth){}

short MiniMaxAgent::maxMove(Board &board, unsigned short depth, short alpha, short beta) const
{
    // if game over return score.
    BoardState state = board.evaluateBoard();
    if ( (0 == depth) || (BoardState::NoWinner != state) )
        return score(state);

    // Choose the best score given all empty cell choices.
    short bestScore = SHRT_MIN;
    for (size_t row = 0; row < board.size(); ++row) {
        for (size_t col = 0; col < board.size(); ++col) {
            if (BoardMarks::Empty == board.at(row, col)) {
                // Try the move
                board.setPlayerInput(row, col, mark_);

                // Compare result of this move with respect to AI.
                short score = minMove(board, depth - 1, alpha, beta);

                // Reset the move done.
                board.resetCell(row, col);

                // Update the best score.
                bestScore = std::max(bestScore, score);
                // Update alpha
                alpha = std::max(alpha, score);
                // Prune the rest of cells after this one.
                if (beta <= alpha)
                    break;
            }
        }
    }
    return bestScore;
}

short MiniMaxAgent::minMove(Board &board, unsigned short depth, short alpha, short beta) const
{
    // if game over return score
    BoardState state = board.evaluateBoard();
    if ( (0 == depth) || (BoardState::NoWinner != state) )
        return score(state);

    short bestScore = SHRT_MAX;
    for (size_t row = 0; row < board.size(); ++row) {
        for (size_t col = 0; col < board.size(); ++col) {
            if (BoardMarks::Empty == board.at(row, col)) {
                // Try the move
                board.setPlayerInput(row, col, playerMark_());

                // Compare result of this move with respect to player
                short score = maxMove(board, depth - 1, alpha, beta);

                // Reset the move done
                board.resetCell(row, col);

                // Update the best score.
                bestScore = std::min(bestScore, score);
                // Update beta
                beta = std::min(beta, score);
                // Prune the rest of cells after this one.
                if (beta <= alpha)
                    break;
            }
        }
    }
    return bestScore;
}

short MiniMaxAgent::score(const BoardState state) const
{
    // AI win score.
    if ( (BoardMarks::O == mark_) && (BoardState::OWins == state) )
        return AI_WIN_SCORE;
    if ( (BoardMarks::X == mark_) && (BoardState::XWins == state) )
        return AI_WIN_SCORE;
    // Player win score.
    if ( (BoardMarks::O == mark_) && (BoardState::XWins == state) )
        return PLAYER_WIN_SCORE;
    if ( (BoardMarks::X == mark_) && (BoardState::OWins == state) )
        return PLAYER_WIN_SCORE;

    // Tie or a non final state score.
    return TIE_SCORE;
}

int MiniMaxAgent::play()
{
    Board copy_ = this->*board;
    // No play if the board is at a final state.
    if (BoardState::NoWinner != copy_.evaluateBoard())
        return defaults::INVALID_CELL;

    // Start of the minimax algorith and choose the best score of all available cells.
    int bestScore = INT_MIN;
    QPair<size_t, size_t> bestEntry;
    for (size_t row = 0; row < copy_.size(); ++row) {
        for (size_t col = 0; col < copy_.size(); ++col) {
            if (BoardMarks::Empty == copy_.at(row, col)) {
                // Try the move
                copy_.setPlayerInput(row, col, mark_);

                // Update the best score and the best cell location.
                int moveScore = minMove(copy_, depth_ - 1, SHRT_MIN, SHRT_MAX);
                if (moveScore > bestScore) {
                    bestScore = moveScore;
                    bestEntry.first = row;
                    bestEntry.second = col;
                }

                // Reset the move done
                copy_.resetCell(row, col);
            }
        }
    }

    // Set the AI choice on the board.
    copy_.setPlayerInput(bestEntry.first, bestEntry.second, mark_);
    // Return the 1D index of the cell to delegate any other updates needed.
    return static_cast<int>(bestEntry.first * copy_.size() + bestEntry.second);
}
