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
 * @brief Clase gestora del reconocimiento de voz utilizando Pocketsphinx y PortAudio.
 *
 * Esta clase encapsula toda la lógica para la captura de audio (PortAudio) y
 * el procesamiento del reconocimiento de voz (Pocketsphinx) en un hilo de trabajo
 * separado.
 *
 * Emite señales cuando el discurso es reconocido, permitiendo la comunicación
 * segura con el hilo principal (Main Thread).
 */
class SpeechManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor de la clase SpeechManager.
     *
     * Inicializa los punteros de los recursos de PortAudio y Pocketsphinx a nullptr
     * y configura el objeto para la gestión de hilos.
     *
     * @param parent Puntero al objeto padre (QObject).
     */
    explicit SpeechManager(QObject *parent = nullptr);

    /**
     * @brief Destructor de la clase SpeechManager.
     *
     * Garantiza la liberación de todos los recursos de Pocketsphinx y PortAudio
     * (`ps_free`, `Pa_Terminate`, etc.) para evitar pérdidas de memoria.
     */
    ~SpeechManager();

    /**
     * @brief Configura los parámetros iniciales de Pocketsphinx.
     *
     * Esta función se encarga de cargar los modelos de lenguaje, el diccionario y
     * los parámetros acústicos en la estructura `ps_config_t`.
     * Debe llamarse antes de `init()`.
     */
    void configure();

    /**
     * @brief Inicializa el decodificador de Pocketsphinx y PortAudio.
     *
     * Crea e inicializa el decodificador (`ps_decoder_t`) con la configuración
     * establecida en `configure()` y prepara la captura de audio de PortAudio.
     */
    void init();

    /**
     * @brief Inicia la captura de audio y el bucle de reconocimiento de voz.
     *
     * Abre el *stream* de audio de PortAudio e inicia el proceso de reconocimiento
     * de voz continuo de Pocketsphinx. Debe ser llamado desde el hilo de trabajo.
     */
    void startListening();

signals:
    /**
     * @brief Señal emitida cuando una frase o comando ha sido reconocido.
     * @param text La cadena de texto reconocida (la hipótesis).
     */
    void speechRecognized(const QString &text);

    /**
     * @brief Señal emitida cuando Pocketsphinx devuelve una nueva hipótesis.
     *
     * Se utiliza principalmente para la comunicación interna entre métodos
     * del worker thread antes de enviar el resultado al hilo principal.
     *
     * @param hyp La hipótesis de reconocimiento como una cadena de caracteres C.
     */
    void speechReadyToProcess(const char *hyp);

private slots:
    /**
     * @brief Procesa el resultado de la hipótesis devuelta por Pocketsphinx.
     *
     * Este slot se encarga de realizar una validación final y emitir la señal
     * `speechRecognized` si la hipótesis es válida.
     *
     * @param hyp La cadena de texto reconocida.
     */
    void processRecognitionResult(const char *hyp);

private:
    PaStream *stream_;           ///< Puntero al *stream* de audio de PortAudio.
    PaError err_;                ///< Código de error de PortAudio.
    ps_decoder_t *decoder_;      ///< Puntero principal al decodificador de Pocketsphinx.
    ps_config_t *config_;        ///< Configuración utilizada por el decodificador.
    ps_endpointer_t *ep_;        ///< Detector de final de voz (VAD) de Pocketsphinx.
    short *frame_;               ///< *Buffer* para almacenar fragmentos de audio.
    size_t frame_size_;          ///< Tamaño de los fragmentos de audio.
    const char *hyp_;            ///< La hipótesis de reconocimiento actual.

    /**
     * @brief Verifica si la hipótesis de reconocimiento es válida.
     *
     * Comprueba si la hipótesis no es nula y cumple con los requisitos mínimos
     * de confianza o longitud para ser considerada un comando.
     * @return true si la hipótesis es válida.
     */
    bool hypIsValid();

    /**
     * @brief Libera la memoria asignada en el *heap* (montón).
     *
     * Función auxiliar utilizada por el destructor para liberar todos los punteros
     * de Pocketsphinx y PortAudio.
     */
    void freeHeap();
};
#endif // SPEECHMANAGER_H
