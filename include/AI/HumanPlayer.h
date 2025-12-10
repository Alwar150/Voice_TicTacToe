#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H
#include <Player.h>
#include <SpeechManager.h>
#include <QThread>

/**
 * @file HumanPlayer.h
 * @brief Definition of the HumanPlayer class that represents the human player.
 * @author Miguel Fernández Lorenzo
 * @version 1.0
 * @date November 2025
 */

/**
 * @class HumanPlayer
 * @brief The HumanPlayer class represents the human player in the game.
 *
 * Inherits from the abstract base class Player. It is responsible for capturing
 * user input (either through GUI clicks or, in this case, through voice
 * recognition using SpeechManager) and converting it into a valid move on the board.
 *
 * Uses a dedicated worker thread (`QThread`) for voice processing
 * (`SpeechManager`) to avoid blocking the main GUI thread.
 *
 * @startuml
 * class HumanPlayer extends Player
 * --
 * -stt_: SpeechManager*
 * -sttThread_: QThread*
 * --
 * +HumanPlayer(const Board&, BoardMarks, QObject*)
 * +~HumanPlayer()
 * +play(): void
 * +<<signal>>  startListening()
 * +<<signal>>  stopListening()
 * +<<signal>> playerFinished(int)
 * --
 * Player ..|> QObject
 * HumanPlayer ..|> Player
 * @enduml
 */

 
class HumanPlayer final : public Player{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the human player.
     *
     * Initializes the reference to the board and the player's marker. Also
     * creates and initializes the SpeechManager object and its associated thread.
     *
     * @param board_ref Constant reference to the board model.
     * @param playerMark The marker (BoardMarks::X or BoardMarks::O) assigned to the player.
     * @param parent Pointer to the parent QObject.
     */
    explicit HumanPlayer(const Board& board_ref, BoardMarks playerMark = BoardMarks::Empty, QObject* parent = nullptr);

    /**
     * @brief Destructor for the human player.
     *
     * Handles the cleanup and safe termination of the voice recognition thread.
     */
    ~HumanPlayer();

    /**
     * @brief Starts the human player's turn.
     *
     * This overridden method starts the voice recognition thread (`SpeechManager`)
     * and instructs it to begin listening, waiting for a user command.
     */
    void play() override;

signals:
    /**
     * @brief Signal emitted to start microphone listening in the worker thread.
     */
    void startListening();

    /**
     * @brief Signal emitted to stop microphone listening and shut down the worker thread.
     */
    void stopListening();

private:
    /**
     * @brief Processes the text recognized by the SpeechManager.
     *
     * This method receives the text hypothesis (`text`) from the voice recognition system
     * and initiates command validation.
     *
     * @param text The recognized text string (voice command).
     */
    void parseSpeechCommand(const QString &text);

    /**
     * @brief Validates whether the recognized voice command corresponds to a valid move.
     *
     * Verifies that the command is understandable (e.g., "play cell 5") and that the
     * indicated cell is available on the board.
     *
     * @param command The text string to validate.
     * @return true if the command is valid and the cell is available.
     */
    bool validateCommand(const QString &command);

    /**
     * @brief Slot responsible for managing the worker thread lifecycle.
     *
     * Executes the safe thread termination process (`quit()`, `wait()`)
     * and resource cleanup (`QThread::deleteLater`).
     */
    void handleThreadCleanup();

    SpeechManager* stt_;    ///< Pointer to the SpeechManager object, the *worker* for voice recognition.
    QThread* sttThread_;    ///< Pointer to the worker thread dedicated to running the SpeechManager.
};

#endif // HUMANPLAYER_H
