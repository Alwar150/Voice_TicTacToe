#include "TTTController.h"
#include "MiniMaxAgent.h"
#include "NoAgent.h"
#include "TicTacToeGame.h"
#include <QPushButton>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>

TTTController::TTTController(const TTTOptions &options, QObject *parent)
    : QObject(parent),
    view_(),
    board_(options.boardSize),
    options_(options),
    currentPlayer_(BoardMarks::X)
{
    // Build GUI
    cells_ = view_.buildCellButtons(options.boardSize);

    // Set Connections to the UI elements.
    setConnections();

    // Specifies the type of agent, and its behaviour throught polymorphism.
    if (options_.AIopponent && options_.AIstarts)
        agent_ = std::make_unique<MiniMaxAgent>(options_.miniMaxDepth, BoardMarks::X, BoardMarks::O);
    else if (options_.AIopponent)
        agent_ = std::make_unique<MiniMaxAgent>(options_.miniMaxDepth, BoardMarks::O, BoardMarks::X);
    else
        agent_ = std::make_unique<NoAgent>();

    // Setup a new game.
    reset();
}

void TTTController::startGame()
{
    // Shows the gameplay GUI.
    view_.show();
}

void TTTController::setConnections()
{

    // New Game Connection - resetting the game.
    connect(&view_, &TicTacToeGame::newGame, this, [&] { reset(); });

    // Connect AI to play after a cell is chosen by human input.
    connect(this, &TTTController::humanFinished,
            this, &TTTController::AIAgentPlay, Qt::QueuedConnection);

    connect(this, &TTTController::AIFinished,
            this, &TTTController::HumanPlay, Qt::QueuedConnection);
    connect(this, &TTTController::speechRecognized,
            this, &TTTController::onSpeechRecognized);

}

void TTTController::updateGameState(Cell &cell)
{
    // Updates the cell view.
    this->view_.updateCell(cell, currentPlayer_);

#ifdef QT_DEBUG
    qDebug() << "Current playter: " << (currentPlayer_ == BoardMarks::X ? "X" : "O");
    board_.printBoard();
#endif
    // Update board state and declare state if its a final state.
    BoardState boardState = board_.evaluateBoard();
    if (BoardState::NoWinner != boardState)
        view_.declareGameState(boardState);

    // Switch the players.
    switchPlayer();
}

void TTTController::reset()
{
    // Resets the current player back to X.
    currentPlayer_ = BoardMarks::X;
    // Resets the View GUI elements.
    view_.reset(cells_);
    // Resets the internal Representation of the board.
    board_.reset();
    // Resets the AI agent.
    // Uses the arrow operator to avoid calling unique_pointer::reset()
    agent_->reset();
    // Start AI play.
    if (options_.AIstarts)
        AIAgentPlay();
    else{
        HumanPlay();
    }
}

void TTTController::AIAgentPlay()
{
    qDebug() << "AIAgentPlay called!";
    int cellIdx = agent_->play(board_);
    qDebug() << "AI tries to play at index:" << cellIdx << "with mark" << (int)currentPlayer_;

    if (cellIdx != defaults::INVALID_CELL) {
        Cell &cell = cells_.at(static_cast<size_t>(cellIdx));
        qDebug() << "Cell status before play:" << (int)board_.at(cell.row, cell.col);
        updateGame(cell);
    } else {
        qDebug() << "AI returned INVALID_CELL!";
    }
}

void TTTController::HumanPlay()
{
    // Crea una instancia en heap (para no destruirla al salir del scope)
    SpeechToText *stt = new SpeechToText();
    stt->configuration();

    // Ejecuta el reconocimiento en un hilo aparte
    QFuture<void> future = QtConcurrent::run([this]() {
        SpeechToText stt;
        stt.configuration();
        stt.init();
        stt.listen();

        QString hyp = QString::fromUtf8(stt.hyp);

        // Enviar de forma segura al hilo principal:
        QMetaObject::invokeMethod(this, [this, hyp]() {
            emit speechRecognized(hyp);
        }, Qt::QueuedConnection);

        stt.free();
    });

}

void TTTController::switchPlayer()
{
    if (BoardMarks::X == currentPlayer_)
        currentPlayer_ = BoardMarks::O;
    else if (BoardMarks::O == currentPlayer_)
        currentPlayer_ = BoardMarks::X;
}

void TTTController::updateGame(Cell &cell)
{
    bool success = board_.setPlayerInput(static_cast<size_t>(cell.row),
                                         static_cast<size_t>(cell.col),
                                         currentPlayer_);
    if (success) {
        BoardMarks prevPlayer = currentPlayer_;
        updateGameState(cell);

        qDebug() << "PrevPlayer:" << (prevPlayer == BoardMarks::X ? "X" : "O")
                 << ", currentPlayer:" << (currentPlayer_ == BoardMarks::X ? "X" : "O");

        if (options_.AIstarts && prevPlayer == BoardMarks::X) {
            qDebug() << "Emit AIFinished";
            emit AIFinished();
            return;
        }
        else if (!options_.AIstarts && prevPlayer == BoardMarks::X) {
            qDebug() << "Emit humanFinished";
            emit humanFinished();
            return;
        }
        else if (options_.AIstarts && prevPlayer == BoardMarks::O) {
            qDebug() << "Emit humanFinished";
            emit humanFinished();
            return;
        }
        else {
            qDebug() << "Emit AIFinished";
            emit AIFinished();
            return;
        }
    }

}

void TTTController::onSpeechRecognized(const QString &text)
{
#ifdef QT_DEBUG
    qDebug() << "Reconocido:" << text;
#endif

    // Aquí conviertes la frase en cellIdx como antes
    int cellIdx = parseSpeechToCell(text);
    if (cellIdx != defaults::INVALID_CELL) {
        updateGame(cells_.at(static_cast<size_t>(cellIdx)));
    } else {
        // Si no entendió, pedir otra vez
        HumanPlay();
    }
}

void TTTController::setupNetwork() {
    socket_ = new QTcpSocket(this);
    socket_->connectToHost("192.168.1.50", 5000); // IP de RobotStudio o servidor
    connect(socket_, &QTcpSocket::connected, []() {
        qDebug() << "Conectado al servidor.";
    });
    connect(socket_, &QTcpSocket::errorOccurred, [](QAbstractSocket::SocketError err) {
        qWarning() << "Error TCP:" << err;
    });
}

void TTTController::sendMove(int row, int col, char player) {
    if (socket_ && socket_->isOpen()) {
        QString msg = QString("MOVE %1 %2 %3\n").arg(row).arg(col).arg(player);
        socket_->write(msg.toUtf8());
    }
}
