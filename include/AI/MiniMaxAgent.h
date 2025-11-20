#ifndef MINIMAXAGENT_H
#define MINIMAXAGENT_H
#include "AIAgent.h"

/**
 * @brief La clase MiniMaxAgent implementa la lógica para seleccionar el mejor movimiento mediante el algoritmo Minimax.
 *
 * Hereda de AIAgent (la clase base para agentes de IA). Este agente simula recursivamente
 * todos los posibles movimientos futuros hasta alcanzar un estado final o la profundidad máxima
 * de búsqueda (`depth_`). Utiliza la **poda Alpha-Beta** para optimizar la búsqueda.
 * El algoritmo evalúa los estados finales asignando puntuaciones: positiva para la victoria de la IA,
 * negativa para la victoria del jugador humano, y cero para un empate.
 */
class MiniMaxAgent final : public AIAgent
{
    Q_OBJECT

public:
    /**
     * @brief Constructor de la clase MiniMaxAgent.
     *
     * Inicializa el agente con el tablero, el marcador asignado a la IA y la profundidad
     * máxima para la búsqueda Minimax.
     *
     * @param board Referencia constante al tablero del juego.
     * @param AImark El marcador (BoardMarks::X o BoardMarks::O) asignado a la IA.
     * @param depth La profundidad máxima de búsqueda (cutoff) para el algoritmo Minimax.
     * @param parent Puntero al objeto QObject padre.
     */
    explicit MiniMaxAgent(const Board& board, BoardMarks AImark, unsigned short depth, QObject* parent = nullptr);

    /**
     * @brief Inicia el algoritmo Minimax para determinar y ejecutar el mejor movimiento de la IA.
     *
     * Este método es la implementación de la función abstracta de la clase `Player`.
     * Calcula la mejor celda y emite la señal `playerFinished` con el índice de la celda elegida.
     */
    void play() override;

private:
    /**
     * @brief Profundidad máxima de búsqueda del árbol del algoritmo Minimax.
     *
     * Define el límite de cuántos movimientos futuros simulará el agente.
     */
    const unsigned short depth_;

    /**
     * @brief Devuelve el marcador del jugador humano.
     *
     * Determina automáticamente el marcador del jugador oponente basándose en el marcador de la IA.
     * @return El marcador (BoardMarks::X o BoardMarks::O) del jugador humano.
     */
    inline BoardMarks playerMark_() const {
        return (BoardMarks::X == mark_) ? BoardMarks::O : BoardMarks::X;
    }

    /**
     * @brief Puntuación asignada al estado final cuando la IA gana la partida.
     */
    static constexpr short AI_WIN_SCORE = 1;

    /**
     * @brief Puntuación asignada al estado final cuando el jugador humano gana la partida.
     */
    static constexpr short PLAYER_WIN_SCORE = -1;

    /**
     * @brief Puntuación asignada al estado final cuando el juego termina en empate.
     */
    static constexpr short TIE_SCORE = 0;

    /**
     * @brief Función del nodo maximizador (turno de la IA).
     *
     * Simula el turno de la IA, eligiendo la jugada que maximice la puntuación final
     * y aplicando la poda Alpha-Beta.
     *
     * @param board Referencia al modelo del tablero (se modifica temporalmente durante la simulación).
     * @param depth La profundidad actual del árbol de búsqueda.
     * @param alpha El valor mínimo garantizado para el maximizador (IA).
     * @param beta El valor máximo garantizado para el minimizador (Jugador).
     * @return La máxima puntuación posible para el estado actual.
     */
    short maxMove(Board &board, unsigned short depth, short alpha, short beta) const;

    /**
     * @brief Función del nodo minimizador (turno del jugador humano).
     *
     * Simula el turno del jugador humano, eligiendo la jugada que minimice la puntuación final
     * de la IA y aplicando la poda Alpha-Beta.
     *
     * @param board Referencia al modelo del tablero (se modifica temporalmente durante la simulación).
     * @param depth La profundidad actual del árbol de búsqueda.
     * @param alpha El valor mínimo garantizado para el maximizador (IA).
     * @param beta El valor máximo garantizado para el minimizador (Jugador).
     * @return La mínima puntuación posible para el estado actual.
     */
    short minMove(Board &board, unsigned short depth, short alpha, short beta) const;

    /**
     * @brief Asocia una puntuación a un estado final del juego.
     *
     * Evalúa el estado del tablero determinando si hay un ganador, un perdedor o un empate.
     *
     * @param state El estado final del juego (BoardState).
     * @return Una puntuación basada en el estado del juego (AI_WIN_SCORE, PLAYER_WIN_SCORE, TIE_SCORE).
     */
    short score(const BoardState state) const;
};

#endif // MINIMAXAGENT_H
