#include "TTTController.h"
#include "MiniMaxAgent.h"
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

    // Specifies the type of agent, and its behaviour throught polymorphism.
    if (options_.AIopponent){
        if(options_.AIstarts){
            playerX_ = std::make_unique<MiniMaxAgent>(board_,BoardMarks::X,options_.miniMaxDepth,this);
            playerO_ = std::make_unique<HumanPlayer>(board_,BoardMarks::O,this);
            connect(dynamic_cast<HumanPlayer*>(playerO_.get()), &HumanPlayer::startListening,
                    this, [this]() {
                        static_cast<TicTacToeGame*>(this->getView())->updateRecordingStatus(true);
                    });

            connect(dynamic_cast<HumanPlayer*>(playerO_.get()), &HumanPlayer::stopListening,
                    this, [this]() {
                        static_cast<TicTacToeGame*>(this->getView())->updateRecordingStatus(false);
                    });
        } else {
            playerO_ = std::make_unique<MiniMaxAgent>(board_,BoardMarks::O,options_.miniMaxDepth,this);
            playerX_ = std::make_unique<HumanPlayer>(board_,BoardMarks::X,this);
            connect(dynamic_cast<HumanPlayer*>(playerX_.get()), &HumanPlayer::startListening,
                    this, [this]() {
                        static_cast<TicTacToeGame*>(this->getView())->updateRecordingStatus(true);
                    });

            connect(dynamic_cast<HumanPlayer*>(playerX_.get()), &HumanPlayer::stopListening,
                    this, [this]() {
                        static_cast<TicTacToeGame*>(this->getView())->updateRecordingStatus(false);
                    });
        }
    } else {
        playerX_ = std::make_unique<HumanPlayer>(board_,BoardMarks::X,this);
        playerO_ = std::make_unique<HumanPlayer>(board_,BoardMarks::O,this);
        connect(dynamic_cast<HumanPlayer*>(playerX_.get()), &HumanPlayer::startListening,
                this, [this]() {
                    static_cast<TicTacToeGame*>(this->getView())->updateRecordingStatus(true);
                });

        connect(dynamic_cast<HumanPlayer*>(playerX_.get()), &HumanPlayer::stopListening,
                this, [this]() {
                    static_cast<TicTacToeGame*>(this->getView())->updateRecordingStatus(false);
                });
        connect(dynamic_cast<HumanPlayer*>(playerO_.get()), &HumanPlayer::startListening,
                this, [this]() {
                    static_cast<TicTacToeGame*>(this->getView())->updateRecordingStatus(true);
                });

        connect(dynamic_cast<HumanPlayer*>(playerO_.get()), &HumanPlayer::stopListening,
                this, [this]() {
                    static_cast<TicTacToeGame*>(this->getView())->updateRecordingStatus(false);
                });
    }
    network_ = new NetworkManager;
    // Set Connections to the UI elements.
    setConnections();
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
    const QString ip = ConfigManager::instance().getValue("Network/server_ip", "127.0.0.1");
    const int port = ConfigManager::instance().getInt("Network/server_port", 5000);

    network_->connectToServer(ip,port);
    //
    // 🔁 Reiniciar el juego desde la interfaz gráfica
    //
    connect(&view_, &TicTacToeGame::newGame, this, [&]() { reset(); });

    //
    // 🧩 Conexiones entre los jugadores y el controlador
    //
    // Cuando playerX_ (IA o humano) termina su turno y emite `playerFinished(int index)`
    // se actualiza el tablero desde el controlador.
    connect(playerX_.get(), &Player::playerFinished,
            this, [this](const int& idx) {
                this->updateGame(cells_.at(static_cast<size_t>(idx)));
            });

    // Igual para el jugador O, pero como su signal tiene el mismo tipo de parámetro,
    // podemos conectar directamente al slot.
    connect(playerO_.get(), &Player::playerFinished,
            this, [this](const int& idx) {
                this->updateGame(cells_.at(static_cast<size_t>(idx)));
            });

    //
    // 🌐 Conexiones de red con NetworkManager
    //
    connect(network_, &NetworkManager::connected, this, [this]() {
        qDebug() << "[CORE::NET] Conectado al servidor.";
        static_cast<TicTacToeGame*>(this->getView())->updateConnectionStatus(true);
    });

    connect(network_, &NetworkManager::disconnected, this, [this]() {
        qDebug() << "[CORE::NET] Desconectado del servidor.";
        static_cast<TicTacToeGame*>(this->getView())->updateConnectionStatus(false);
    });

    connect(network_, &NetworkManager::messageSent, this, [this](const QString& msg) {
        qDebug() << "[CORE::NET] Mensaje enviado:" << msg;
        static_cast<TicTacToeGame*>(this->getView())->updateWaitingStatus(true);
    });

    // Ejemplo: si NetworkManager recibe algo (puedes definir una señal messageReceived(QString))
    connect(network_, &NetworkManager::messageReceived,this,[this](const QString& msg){
        qDebug() << "[CORE::NET] Mensaje recibido:" << msg;
        &TTTController::onNetworkMessageReceived;
        // We don't check the content of the message
        static_cast<TicTacToeGame*>(this->getView())->updateWaitingStatus(false);
    });
}

void TTTController::updateGameState(Cell &cell)
{
    // Updates the cell view.
    this->view_.updateCell(cell, currentPlayer_);

#ifdef QT_DEBUG
    qDebug() << "[CORE] Current player: " << (currentPlayer_ == BoardMarks::X ? "X" : "O");
    board_.printBoard();
#endif
    // Update board state and declare state if its a final state.
    BoardState boardState = board_.evaluateBoard();
    if (BoardState::NoWinner != boardState)
        view_.declareGameState(boardState);
}

void TTTController::reset()
{
    // Resets the current player back to X.
    currentPlayer_ = BoardMarks::X;
    // Resets the View GUI elements.
    view_.reset(cells_);
    // Resets the internal Representation of the board.
    board_.reset();

    // Start.
    playerX_->play();
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

        // Network send move to server
        {
            QString msg = "";
            msg = QString::number(cell.row) + "," + QString::number(cell.col) + "," + QString(currentPlayer_ == BoardMarks::X ? "X" : "O");
            network_->sendMessage(msg);
        }

    }
}

void TTTController::onNetworkMessageReceived(const QString& msg){
    if(msg == "DONE"){
        if(board_.evaluateBoard() == BoardState::NoWinner){
            switchPlayer();
            if(currentPlayer_ == BoardMarks::X){
                playerX_->play();
            }else{
                playerO_->play();
            }
        } else {
            reset();
        }
    }
}

