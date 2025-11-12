#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(QObject& parent = nullptr, const Board& board_ref, BoardMarks playerMark = BoardMarks::Empty) 
: Player(parent, playerMark, board_ref){
    stt_ = new SpeechManager(this);
    sttThread_ = nullptr;
    stt_->configure();
    // 1. Conectar la señal del hilo principal (HumanPlayer) con el slot de SpeechManager
    // startListening() -> slot startSpeechRecognition() en el hilo worker
    connect(this, &HumanPlayer::startListening,
            speechManager_, &SpeechManager::startListening,
            Qt::QueuedConnection);

    // 2. Conectar la señal de SpeechManager (hipótesis reconocida) con el slot de HumanPlayer
    // Esto se ejecuta en el hilo principal
    connect(speechManager_, &SpeechManager::speechRecognized,
            this, [this]() {
                QString hyp = speechManager_->getCurrentHyp(); // obtener hipótesis
                QMetaObject::invokeMethod(this, "onSpeechRecognized",
                                          Qt::QueuedConnection,
                                          Q_ARG(QString, hyp));
            });

    // 3. Conectar señal para detener la escucha y devolver SpeechManager al hilo principal
    connect(this, &HumanPlayer::stopListening, speechManager_, [this]() {
        speechThread_->quit();
        speechThread_->wait();  // Espera a que termine el hilo
        speechManager_->moveToThread(this->thread()); // devuelve al hilo principal
    });

    void HumanPlayer::play(){
        sttThread_ = new QThread(this);
        stt_->moveToThread(sttThread_);
        sttThread_->start();
        emit startListening();
    }

    void HumanPlayer::onSpeechRecognized(const QString &text){
        int cell = parseSpeechCommand(text);
        if (cell != -1) {
            emit playerFinished(cell);
            emit stopListening();
        } else {
            qDebug() << "[HumanPlayer] Comando no válido:" << text;
            emit startListening(); // Reiniciar escucha
        }
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
        } else if (first == "centro" || second == "centro") {
            row = 1;
        } else if (first == "abajo" || second == "abajo") {
            row = 2;
        }
        // Determinar columna
        if (first == "izquierda" || second == "izquierda") {
            col = 0;
        } else if (first == "centro" || second == "centro") {
            col = 1;
        } else if (first == "derecha" || second == "derecha") {
            col = 2;
        }
        if(board_->at(row, col) != BoardMarks::EMPTY){
            emit startListening();
            return; // Celda ya ocupada
        }
        else{
            emit stopListening();
            emit playerFinished(row * board_->size() + col);
        }
    }