#include "MiniMaxAgent.h"
#include <limits>
#include <algorithm>

// Definimos puntuaciones altas para que la profundidad tenga efecto.
// Esto hace que la IA sea "más lista" que la de la referencia básica.
static constexpr short MAX_SCORE = 1000;
static constexpr short MIN_SCORE = -1000;

MiniMaxAgent::MiniMaxAgent(const Board& board, BoardMarks AImark, unsigned short depth, QObject* parent)
    : AIAgent(AImark, board, parent), depth_(depth)
{
    // A diferencia de la referencia, tú no necesitas guardar playerMark_
    // porque lo calculas dinámicamente con tu función inline playerMark_()
}

short MiniMaxAgent::maxMove(Board &board, unsigned short depth, short alpha, short beta) const
{
    BoardState state = board.evaluateBoard();

    // Condición de parada
    if ((0 == depth) || (BoardState::NoWinner != state)) {
        return score(state);
    }

    short bestScore = std::numeric_limits<short>::min();

    for (size_t row = 0; row < board.size(); ++row) {
        for (size_t col = 0; col < board.size(); ++col) {
            if (BoardMarks::Empty == board.at(row, col)) {

                // 1. Hacer movimiento
                board.setPlayerInput(row, col, mark_);

                // 2. Llamada recursiva
                short val = minMove(board, depth - 1, alpha, beta);

                // 3. DESHACER MOVIMIENTO (Clave de la referencia)
                board.resetCell(row, col);

                // 4. Maximizar
                bestScore = std::max(bestScore, val);
                alpha = std::max(alpha, bestScore);

                // Poda
                if (beta <= alpha) return bestScore;
            }
        }
    }
    return bestScore;
}

short MiniMaxAgent::minMove(Board &board, unsigned short depth, short alpha, short beta) const
{
    BoardState state = board.evaluateBoard();

    if ((0 == depth) || (BoardState::NoWinner != state)) {
        return score(state);
    }

    short bestScore = std::numeric_limits<short>::max();

    for (size_t row = 0; row < board.size(); ++row) {
        for (size_t col = 0; col < board.size(); ++col) {
            if (BoardMarks::Empty == board.at(row, col)) {

                // 1. Hacer movimiento (usamos tu función helper playerMark_())
                board.setPlayerInput(row, col, playerMark_());

                // 2. Llamada recursiva
                short val = maxMove(board, depth - 1, alpha, beta);

                // 3. DESHACER MOVIMIENTO (Clave de la referencia)
                board.resetCell(row, col);

                // 4. Minimizar
                bestScore = std::min(bestScore, val);
                beta = std::min(beta, bestScore);

                // Poda
                if (beta <= alpha) return bestScore;
            }
        }
    }
    return bestScore;
}

short MiniMaxAgent::score(const BoardState& state) const
{
    // Evaluación simple del estado final
    if ((BoardMarks::O == this->mark_) && (BoardState::OWins == state)){
        return AI_WIN_SCORE;
    }
    if ((BoardMarks::X == this->mark_) && (BoardState::XWins == state)){
        return AI_WIN_SCORE;
    }

    if ((BoardMarks::O == this->mark_) && (BoardState::XWins == state)){
        return PLAYER_WIN_SCORE;
    }
    if ((BoardMarks::X == this->mark_) && (BoardState::OWins == state))
    {
        return PLAYER_WIN_SCORE;
    }

    return TIE_SCORE;
}

void MiniMaxAgent::play()
{
    // ADAPTACIÓN: La referencia recibe board por parámetro.
    // Tú usas 'this->board_' (del padre AIAgent), por lo que debemos hacer una COPIA
    // para no alterar el juego real durante la simulación.
    Board copy_(*this->board_);

    // Verificación de seguridad
    if (BoardState::NoWinner != copy_.evaluateBoard()) {
        emit playerFinished(-1);
        return;
    }

    short bestScore = std::numeric_limits<short>::min();
    int bestIndex = -1;

    // Este bucle es lógicamente IDÉNTICO al de la referencia
    for (size_t row = 0; row < copy_.size(); ++row) {
        for (size_t col = 0; col < copy_.size(); ++col) {
            if (BoardMarks::Empty == copy_.at(row, col)) {

                // A. Probar movimiento en la copia
                copy_.setPlayerInput(row, col, mark_);

                // B. Calcular puntuación
                short moveScore = minMove(copy_, depth_ - 1, std::numeric_limits<short>::min(), std::numeric_limits<short>::max());

                // C. IMPORTANTE: Resetear la celda en la copia
                copy_.resetCell(row, col);

                // D. Elegir mejor movimiento
                if (moveScore > bestScore) {
                    bestScore = moveScore;
                    bestIndex = static_cast<int>(row * copy_.size() + col);
                }

            }
        }
    }

    // ADAPTACIÓN: La referencia retorna int. Tú emites una señal.
    emit playerFinished(bestIndex);
}
