# Tic-Tac-Toe Avanzado: Control por Voz y Red 🎙️🌐

![banner](https://user-images.githubusercontent.com/44305804/84823159-300eb880-b01e-11ea-9f2c-db8b899586a1.png)

Un proyecto Qt en C++ que implementa un juego de Tic-Tac-Toe con **Control por Voz** e integración de **Red TCP/IP**.

---

## 🏗️ Diagrama de Clases (Simplificado)


---

## ⚙️ Componentes del Sistema

| Componente | Rol Principal | Notas |
| :--- | :--- | :--- |
| **TitleScreen** | Punto de Entrada / Configuración | Recolecta opciones de juego (IA, Red) y crea el `TTTController`. |
| **TTTController** | **Controlador (C de MVC)** | Orquesta el flujo, gestiona turnos, y comunica la lógica entre `Player`, `Board` y la red. |
| **Board** | **Modelo (M de MVC)** | Representación del estado 2D del tablero. Verifica victorias y empates. |
| **TicTacToeGame** | **Vista (V de MVC)** | Renderiza la GUI, construye las celdas del tablero y maneja la interacción visual. |
| **Player** | Clase Base Abstracta | Define la interfaz (`play()`) para todos los jugadores. |
| **HumanPlayer** | Jugador Humano (Voz) | Gestiona el turno humano y la **integración con `SpeechManager` en su propio hilo**. |
| **SpeechManager** | Worker de Voz (Pocketsphinx) | Encapsula la captura de audio (PortAudio) y el reconocimiento de comandos de voz en un **`QThread` dedicado**. |
| **NetworkManager** | Comunicaciones TCP/IP | Gestiona la conexión y la transferencia de movimientos en modo multijugador en red. |
| **AIAgent** | Interfaz de Agentes IA | Interfaz base para cualquier algoritmo de inteligencia artificial. |
| **MiniMaxAgent** | Implementación Minimax | Contiene la lógica del algoritmo Minimax con Poda Alpha-Beta. |
| **NoAgent** | Null Object | Representa la ausencia de IA, simplificando la lógica de turnos del controlador. |

---

## 🎨 Diseño y Patrones

### Flujo del Sistema
El `TTTController` coordina el flujo. Durante el turno de un jugador: si es **`HumanPlayer`**, inicia el hilo de `SpeechManager` para escuchar un comando de voz. Si es **`MiniMaxAgent`**, calcula el movimiento. Si es un jugador **Remoto**, espera la señal de `NetworkManager`. El controlador valida el movimiento y actualiza el `Board` y `TicTacToeGame`.

### Patrones de Diseño Utilizados

* **MVC (Modelo-Vista-Controlador):** Implementado para lograr una fuerte **separación de responsabilidades** (Modelo: `Board`, Vista: `TicTacToeGame`, Controlador: `TTTController`).
* **Null Object:** Usado por la clase `NoAgent` para evitar comprobaciones condicionales en el `TTTController` sobre la existencia de un agente IA.
* **Observer Pattern (Señales y Slots):** Principal mecanismo de comunicación de Qt, esencial para la comunicación **segura entre hilos** (e.g., `SpeechManager` enviando resultados a `HumanPlayer`).
* **Worker Thread Pattern (`moveToThread`):** Implementado en `HumanPlayer` para mover `SpeechManager` a un `QThread` dedicado, asegurando que las operaciones de audio y Pocketsphinx no bloqueen la GUI.

---

## 🧠 Algoritmo Minimax

El **`MiniMaxAgent`** utiliza el algoritmo Minimax para determinar el movimiento óptimo. Este algoritmo simula el árbol de juego hasta un estado final (o profundidad máxima), puntúa los resultados y elige la mejor opción asumiendo que el oponente juega de forma óptima.

### Técnicas de Optimización:

* **Poda Alpha-Beta (Alpha Beta Pruning):** Optimiza la búsqueda podando ramas del árbol que se sabe que no afectarán la decisión final. Mantiene valores $\alpha$ (mínimo garantizado para la IA) y $\beta$ (máximo garantizado para el oponente). Si $\beta < \alpha$, se poda.
* **Corte por Profundidad (Depth Cutoff):** Limita la expansión del árbol de búsqueda. Es configurable para controlar la dificultad y el rendimiento, especialmente útil en tableros más grandes.

---

## 🧪 Testing

El proyecto incluye un subproyecto de pruebas unitarias (`./TicTacToeTest/`) que utiliza la librería **Google Test** (v1.10.0).

Para ejecutar las pruebas, se debe configurar la ruta de la librería en `TicTacToeTest.pro`:

```cpp
// TicTacToeTest.pro
// Cambiar la ruta a tu repositorio de Google test
GOOGLETEST_DIR = ../../Tools/googletest-release-1.10.0