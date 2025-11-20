#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(const Board& board_ref, BoardMarks playerMark,QObject* parent)
: Player(playerMark, board_ref, parent){
    stt_ = new SpeechManager();
    sttThread_ = nullptr;
    stt_->configure();
    // 1. Conectar la señal del hilo principal (HumanPlayer) con el slot de SpeechManager
    // startListening() -> slot startSpeechRecognition() en el hilo worker
    connect(this, &HumanPlayer::startListening,
            stt_, &SpeechManager::startListening,
            Qt::QueuedConnection);

    // 3. Conectar señal para detener la escucha y devolver SpeechManager al hilo principal
    connect(this, &HumanPlayer::stopListening, stt_, [this]() {
        sttThread_->quit();
        sttThread_->wait();  // Espera a que termine el hilo
        stt_->moveToThread(this->thread()); // devuelve al hilo principal
    });

    connect(stt_, &SpeechManager::speechRecognized, this, &HumanPlayer::parseSpeechCommand);
}


    void HumanPlayer::play(){
        sttThread_ = new QThread(this);
        stt_->moveToThread(sttThread_);
        sttThread_->start();
        emit startListening();
    }

    void HumanPlayer::parseSpeechCommand(const QString &text){
        QString command = text.trimmed().toLower();
        // text es un String con dos palabras: combinaciones de "centro", "arriba", "abajo", "izquierda", "derecha"
        // Pueden estar en cualquier orden
        QStringList words = command.split(' ', Qt::SkipEmptyParts);
        if (words.size() != 2) {
            emit startListening();
            return; // Comando no válido
        }
        QString first = words[0];
        QString second = words[1];
        int row = -1;
        int col = -1;
        // Determinar fila
        if (first == "arriba" || second == "arriba") {
            row = 0;
        } else if (first == "abajo" || second == "abajo") {
            row = 2;
        } else if (first == "centro" || second == "centro") {
            row = 1;
        }
        // Determinar columna
        if (first == "izquierda" || second == "izquierda") {
            col = 0;
        } else if (first == "derecha" || second == "derecha") {
            col = 2;
        } else if (first == "centro" || second == "centro") {
            col = 1;
        }
        if(board_->at(row, col) != BoardMarks::Empty){
            emit startListening();
            return; // Celda ya ocupada
        }
        else{
            emit stopListening();
            emit playerFinished(row * board_->size() + col);
        }
    }
