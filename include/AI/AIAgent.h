#ifndef AIAGENT_H
#define AIAGENT_H
#include "Player.h"
/**
 * @brief La interfaz AIAgent describe el comportamiento de cualquier agente de Inteligencia Artificial implementado.
 *
 * Esta clase base abstracta hereda de Player y establece una interfaz común
 * para todos los agentes de IA (como MiniMaxAgent). Define las funciones
 * básicas necesarias para que un agente reinicie su estado interno antes
 * de un nuevo turno o juego.
 */

class Board; // Declaración anticipada (forward declaration)

class AIAgent : public Player
{
public:
    /**
     * @brief Constructor de la clase base AIAgent.
     *
     * Inicializa el agente con su marcador y una referencia al tablero.
     *
     * @param playerMark El marcador (BoardMarks::X o BoardMarks::O) asignado a la IA.
     * @param borad_ref Referencia constante al objeto Board (Modelo) del juego.
     * @param parent_ Puntero al objeto QObject padre (nullptr por defecto).
     */
    AIAgent(BoardMarks playerMark, const Board& borad_ref, QObject *parent_ = nullptr)
        : Player(playerMark, borad_ref, parent_) {}

    /**
     * @brief Destructor virtual.
     *
     * Permite la correcta destrucción de las clases derivadas a través de un puntero a la clase base.
     */
    virtual ~AIAgent() {}

    /**
     * @brief Función de reinicio que restablece el estado de cualquier algoritmo de búsqueda interno.
     *
     * Este método es llamado típicamente al comienzo de una nueva partida.
     * Las clases derivadas deben implementar este método para limpiar cualquier memoria caché
     * o estructuras de datos utilizadas por sus algoritmos (aunque no es un método abstracto puro aquí,
     * se define la intención de la función).
     */
    virtual void reset() = 0; // Se hace abstracto para forzar la implementación en derivados
};

#endif // AIAGENT_H
