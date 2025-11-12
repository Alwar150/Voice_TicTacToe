#include "SpeechManager.h"
/**
 * @devitem DEV-SPEECH-0003
 * @title SpeechManager bloquea el hilo principal
 * @status Solved
 * @severity High
 * @owner ratty
 * @rationale SpeechManager debe moverse a QThread
 * @related TASK-THREAD-0002
 */

SpeechManager::SpeechManager(QObject *parent) : QObject(parent), stream_(nullptr), decoder_(nullptr), config_(nullptr), ep_(nullptr), frame_(nullptr), frame_size_(512)
{   
    connect(this, &SpeechManager::speechReadyToProcess, this, &SpeechManager::processRecognitionResult);
    connect(this, &SpeechManager::speechRecognized(const QStrin& hyp), parent, onSpeechRecognized);
}
SpeechManager::~SpeechManager() {
    freeHeap();
}

void SpeechManager::configure() {
    auto& config = ConfigManager::instance();

    QString basePath = QDir::currentPath(); // o QCoreApplication::applicationDirPath()
    QString hmmPath  = QDir(basePath).filePath(config.getValue("Speech/hmm").toString());
    QString lmPath   = QDir(basePath).filePath(config.getValue("Speech/lm").toString());
    QString dictPath = QDir(basePath).filePath(config.getValue("Speech/dict").toString());

    ps_config_set_str(config_, "hmm", hmmPath.toUtf8().constData());
    ps_config_set_str(config_, "lm", lmPath.toUtf8().constData());
    ps_config_set_str(config_, "dict", dictPath.toUtf8().constData());
}

void SpeechManager::init() {
    if ((err = Pa_Initialize()) != paNoError)
            E_FATAL("Failed to initialize PortAudio: %s\n",
                    Pa_GetErrorText(err));
        if ((decoder = ps_init(config)) == NULL)
            E_FATAL("PocketSphinx decoder init failed\n");
        if ((ep = ps_endpointer_init(0, 0.0, PS_VAD_LOOSE, 0, 0)) == NULL)
            E_FATAL("PocketSphinx endpointer init failed\n");
        frame_size = ps_endpointer_frame_size(ep);
        if ((frame = (short int*)malloc(frame_size * sizeof(frame[0]))) == NULL)
            E_FATAL_SYSTEM("Failed to allocate frame");
        if ((err = Pa_OpenDefaultStream(&stream, 1, 0, paInt16,
                                        ps_config_int(config, "samprate"),
                                        frame_size, NULL, NULL)) != paNoError)
            E_FATAL("Failed to open PortAudio stream: %s\n",
                    Pa_GetErrorText(err));
        if ((err = Pa_StartStream(stream)) != paNoError)
            E_FATAL("Failed to start PortAudio stream: %s\n",
                    Pa_GetErrorText(err));
        if (signal(SIGINT, catch_sig) == SIG_ERR)
            E_FATAL_SYSTEM("Failed to set SIGINT handler");
}

void SpeechManager::startListening() {
    hyp = nullptr;
        while (!hypIsValid()) {
            const int16 *speech;
            int prev_in_speech = ps_endpointer_in_speech(ep);
            if ((err = Pa_ReadStream(stream, frame, frame_size)) != paNoError) {
                E_ERROR("Error in PortAudio read: %s\n",
                        Pa_GetErrorText(err));
                break;
            }
            speech = ps_endpointer_process(ep, frame);
            if (speech != NULL) {
                if (!prev_in_speech) {
                    fprintf(stderr, "Speech start at %.2f\n",
                            ps_endpointer_speech_start(ep));
                    fflush(stderr); /* For broken MSYS2 terminal */
                    ps_start_utt(decoder);
                }
                if (ps_process_raw(decoder, speech, frame_size, FALSE, FALSE) < 0)
                    E_FATAL("ps_process_raw() failed\n");
                if (!ps_endpointer_in_speech(ep)) {
                    fprintf(stderr, "Speech end at %.2f\n",
                            ps_endpointer_speech_end(ep));
                    fflush(stderr);
                    ps_end_utt(decoder);
                    if ((hyp = ps_get_hyp(decoder, NULL)) != NULL) {
                        printf("%s\n", hyp);
                        fflush(stdout);
                    }
                }
            }
        }
        if ((err = Pa_StopStream(stream)) != paNoError)
            E_FATAL("Failed to stop PortAudio stream: %s\n",
                    Pa_GetErrorText(err));
        if ((err = Pa_Terminate()) != paNoError)
            E_FATAL("Failed to terminate PortAudio: %s\n",
                    Pa_GetErrorText(err));
}

bool SpeechManager::hypIsValid(){
    if (hyp_ != nullptr)
        return true;
    return false;
}

void SpeechManager::freeHeap(){
    ps_endpointer_free(ep_);
    ps_free(decoder_);
}