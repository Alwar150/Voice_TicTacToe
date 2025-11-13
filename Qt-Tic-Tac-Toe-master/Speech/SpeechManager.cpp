#include "SpeechManager.h"
#include "HumanPlayer.h"
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
}
SpeechManager::~SpeechManager() {
    freeHeap();
}

void SpeechManager::configure() {
    auto& config = ConfigManager::instance();
    QString basePath = QDir::currentPath(); // o QCoreApplication::applicationDirPath()
    QString hmmPath  = QDir(basePath).filePath(config.getValue("Speech/hmm"));
    QString lmPath   = QDir(basePath).filePath(config.getValue("Speech/lm"));
    QString dictPath = QDir(basePath).filePath(config.getValue("Speech/dict"));
    config_ = ps_config_init(NULL);
    ps_config_set_str(config_, "hmm", hmmPath.toUtf8());
    ps_config_set_str(config_, "lm", lmPath.toUtf8());
    ps_config_set_str(config_, "dict", dictPath.toUtf8());
}

void SpeechManager::init() {
    if ((err_ = Pa_Initialize()) != paNoError)
            E_FATAL("Failed to initialize PortAudio: %s\n",
                    Pa_GetErrorText(err_));
        if ((decoder_ = ps_init(config_)) == NULL)
            E_FATAL("PocketSphinx decoder init failed\n");
        if ((ep_ = ps_endpointer_init(0, 0.0, PS_VAD_LOOSE, 0, 0)) == NULL)
            E_FATAL("PocketSphinx endpointer init failed\n");
        frame_size_ = ps_endpointer_frame_size(ep_);
        if ((frame_ = (short int*)malloc(frame_size_ * sizeof(frame_[0]))) == NULL)
            E_FATAL_SYSTEM("Failed to allocate frame");
        if ((err_ = Pa_OpenDefaultStream(&stream_, 1, 0, paInt16,
                                        ps_config_int(config_, "samprate"),
                                        frame_size_, NULL, NULL)) != paNoError)
            E_FATAL("Failed to open PortAudio stream: %s\n",
                    Pa_GetErrorText(err_));
        if ((err_ = Pa_StartStream(stream_)) != paNoError)
            E_FATAL("Failed to start PortAudio stream: %s\n",
                    Pa_GetErrorText(err_));
        /*if (signal(SIGINT, catch_sig) == SIG_ERR)
            E_FATAL_SYSTEM("Failed to set SIGINT handler");*/
}

void SpeechManager::startListening() {
    hyp_ = nullptr;
    this->init();

    while (!hypIsValid()) {
        const int16 *speech;
        int prev_in_speech = ps_endpointer_in_speech(ep_);

        if ((err_ = Pa_ReadStream(stream_, frame_, frame_size_)) != paNoError) {
            E_ERROR("Error in PortAudio read: %s\n", Pa_GetErrorText(err_));
            break;
        }

        speech = ps_endpointer_process(ep_, frame_);
        if (speech != NULL) {
            if (!prev_in_speech) {
                fprintf(stderr, "Speech start at %.2f\n", ps_endpointer_speech_start(ep_));
                fflush(stderr); // For broken MSYS2 terminal
                ps_start_utt(decoder_);
            }

            if (ps_process_raw(decoder_, speech, frame_size_, FALSE, FALSE) < 0) {
                E_FATAL("ps_process_raw() failed\n");
            }

            if (!ps_endpointer_in_speech(ep_)) {
                fprintf(stderr, "Speech end at %.2f\n", ps_endpointer_speech_end(ep_));
                fflush(stderr);

                ps_end_utt(decoder_);
                if ((hyp_ = ps_get_hyp(decoder_, NULL)) != NULL) {
                    printf("%s\n", hyp_);
                    fflush(stdout);
                }
            }
        }
    }

    // Después del bucle
    if ((err_ = Pa_StopStream(stream_)) != paNoError) {
        E_FATAL("Failed to stop PortAudio stream: %s\n", Pa_GetErrorText(err_));
    }

    if ((err_ = Pa_Terminate()) != paNoError) {
        E_FATAL("Failed to terminate PortAudio: %s\n", Pa_GetErrorText(err_));
    }
    emit speechReadyToProcess(hyp_);
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

void SpeechManager::processRecognitionResult(const char *hyp){
    QString qt_hyp = QString::fromUtf8(hyp);
    emit speechRecognized(qt_hyp);
}
