#ifndef SPEECHMANAGER_H
#define SPEECHMANAGER_H
#include <QObject>
#include <QString>
#include <QDebug>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>
#include <cstddef>
#include <portaudio.h>
#include <pocketsphinx.h>
#include <signal.h>
#include <ConfigManager.h>

/**
 * @file SpeechManager.h
 * @brief Definition of the SpeechManager class for voice recognition using Pocketsphinx and PortAudio
 * @author Miguel Fernández Lorenzo
 * @version 1.0
 * @date November 2025
 */

/**
 * @brief Voice recognition manager class using Pocketsphinx and PortAudio.
 *
 * This class encapsulates all the logic for audio capture (PortAudio) and
 * voice recognition processing (Pocketsphinx) in a separate worker thread.
 *
 * Emits signals when speech is recognized, allowing safe communication
 * with the main thread.
 *
 * @startuml
 * class SpeechManager
 * --
 * -stream_: PaStream*
 * -decoder_: ps_decoder_t*
 * -config_: ps_config_t*
 * -frame_: short*
 * -frame_size_: size_t
 * -hyp_: const char*
 * --
 * +SpeechManager(QObject* parent = nullptr)
 * +~SpeechManager()
 * +configure(): void
 * +init(): void
 * +startListening(): void
 * --
 * SpeechManager ..|> QObject
 * @enduml
 */
class SpeechManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor of the SpeechManager class.
     *
     * Initializes PortAudio and Pocketsphinx resource pointers to nullptr
     * and configures the object for thread management.
     *
     * @param parent Pointer to the parent object (QObject).
     */
    explicit SpeechManager(QObject *parent = nullptr);

    /**
     * @brief Destructor of the SpeechManager class.
     *
     * Ensures the release of all Pocketsphinx and PortAudio resources
     * (`ps_free`, `Pa_Terminate`, etc.) to prevent memory leaks.
     */
    ~SpeechManager();

    /**
     * @brief Configures the initial Pocketsphinx parameters.
     *
     * This function is responsible for loading language models, the dictionary and
     * acoustic parameters into the `ps_config_t` structure.
     * Must be called before `init()`.
     */
    void configure();

    /**
     * @brief Initializes the Pocketsphinx decoder and PortAudio.
     *
     * Creates and initializes the decoder (`ps_decoder_t`) with the configuration
     * established in `configure()` and prepares PortAudio audio capture.
     */
    void init();

    /**
     * @brief Starts audio capture and the voice recognition loop.
     *
     * Opens the PortAudio audio stream and starts the Pocketsphinx
     * continuous voice recognition process. Must be called from the worker thread.
     */
    void startListening();

signals:
    /**
     * @brief Signal emitted when a phrase or command has been recognized.
     * @param text The recognized text string (the hypothesis).
     */
    void speechRecognized(const QString &text);

    /**
     * @brief Signal emitted when Pocketsphinx returns a new hypothesis.
     *
     * Mainly used for internal communication between methods
     * of the worker thread before sending the result to the main thread.
     *
     * @param hyp The recognition hypothesis as a C character string.
     */
    void speechReadyToProcess(const char *hyp);

private slots:
    /**
     * @brief Processes the hypothesis result returned by Pocketsphinx.
     *
     * This slot is responsible for performing final validation and emitting the
     * `speechRecognized` signal if the hypothesis is valid.
     *
     * @param hyp The recognized text string.
     */
    void processRecognitionResult(const char *hyp);

private:
    PaStream *stream_;           ///< Pointer to the PortAudio audio stream.
    PaError err_;                ///< PortAudio error code.
    ps_decoder_t *decoder_;      ///< Main pointer to the Pocketsphinx decoder.
    ps_config_t *config_;        ///< Configuration used by the decoder.
    ps_endpointer_t *ep_;        ///< Pocketsphinx voice activity detector (VAD).
    short *frame_;               ///< Buffer to store audio fragments.
    size_t frame_size_;          ///< Size of audio fragments.
    const char *hyp_;            ///< The current recognition hypothesis.

    /**
     * @brief Verifies if the recognition hypothesis is valid.
     *
     * Checks if the hypothesis is not null and meets the minimum requirements
     * of confidence or length to be considered a command.
     * @return true if the hypothesis is valid.
     */
    bool hypIsValid();

    /**
     * @brief Frees memory allocated on the heap.
     *
     * Helper function used by the destructor to free all Pocketsphinx
     * and PortAudio pointers.
     */
    void freeHeap();
};
#endif // SPEECHMANAGER_H