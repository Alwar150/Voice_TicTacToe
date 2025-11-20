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


class SpeechManager : public QObject
{
    Q_OBJECT
public:
    explicit SpeechManager(QObject *parent = nullptr);
    ~SpeechManager();
    void configure();
    void init();
    void startListening();  
signals:
    void speechRecognized(const QString &text);
    void speechReadyToProcess(const char *hyp);

private slots:
    void processRecognitionResult(const char *hyp);
private:
    PaStream *stream_;
    PaError err_;
    ps_decoder_t *decoder_;
    ps_config_t *config_;
    ps_endpointer_t *ep_;
    short *frame_;
    size_t frame_size_;
    const char *hyp_;
    bool hypIsValid();
    void freeHeap();
};
#endif // SPEECHMANAGER_H
