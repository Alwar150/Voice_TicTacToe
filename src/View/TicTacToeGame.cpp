#include "TicTacToeGame.h"
#include <QMessageBox>
#include <QPushButton>

TicTacToeGame::TicTacToeGame(QWidget *parent)
    : QDialog(parent), ui(new Ui::TicTacToeGame)
{
    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    // Accedemos al LED (connectionLabel)
    // CRÍTICO: Establecer ancho y alto iguales a 20x20 para asegurar la forma redonda
    ui->connectionLabel->setFixedSize(20, 20);
    ui->connectionLabel->setToolTip("Estado de Conexión: Desconectado");

    ui->RecordLabel->setFixedSize(40, 25);

    ui->WaitingLabel->setFixedSize(50, 20);

    waiter = new WaitingSpinnerWidget(this,true,true);
    updateWaitingStatus(false);

    updateConnectionStatus(false); // Inicializar en rojo y aplicar el estilo redondo
    setConnections();
    this->adjustSize();
}

void TicTacToeGame::setConnections()
{
    // New Game Connection - resetting the game
    connect(ui->reset, &QPushButton::clicked, [=] { emit newGame(); });
    connect(ui->back, &QPushButton::clicked, [=] { emit close();});
}

vector<Cell> TicTacToeGame::buildCellButtons(size_t boardSize)
{
    vector<Cell> cells;
    cells.reserve(boardSize * boardSize);
    for (size_t row = 0; row < boardSize; ++row) {
        for (size_t col = 0; col < boardSize; ++col) {
            // Add buttons to gridLayout
            QPushButton *btn = new QPushButton();
            btn->setProperty("cell", true);
            ui->gridBoard->addWidget(btn,
                                     static_cast<int>(row),
                                     static_cast<int>(col),
                                     defaults::GUI_CELL_ROW_SPAN,
                                     defaults::GUI_CELL_COLUMN_SPAN);
            // Reference to cells
            cells.emplace_back(btn, row, col);
        }
    }
    // Adjusts window size to fit children widgets added dynamically
    adjustSize();

    // Return by value to allow for RVO (Copy Ellision)
    return cells;
}

QString TicTacToeGame::getPlayerText(BoardMarks currentPlayer)
{
    switch (currentPlayer) {
    case BoardMarks::O:
        return "O";
    case BoardMarks::X:
        return "X";
    default:
        return "";
    }
}

QString TicTacToeGame::getPlayerStyleSheet(BoardMarks currentPlayer)
{
    QString color;

    switch (currentPlayer) {
    case BoardMarks::O:
        color = defaults::O_COLOR;
        break;
    case BoardMarks::X:
        color = defaults::X_COLOR;
        break;
    default:
        color = defaults::DEFAULT_COLOR;
        break;
    }

    return QString("font: 50px \"Verdana\";"
                   "min-height: 100px;"
                   "max-height: 100px;"
                   "min-width: 100px;"
                   "max-width: 100px;"
                   "color: %1")
        .arg(color);
}

void TicTacToeGame::updateCell(Cell &cell, BoardMarks currentPlayer)
{
    // Update Cell button in GUI
    cell.cellBtn->setStyleSheet(getPlayerStyleSheet(currentPlayer));
    cell.cellBtn->setText(getPlayerText(currentPlayer));
}

QString TicTacToeGame::getBoardFinalStateText(BoardState boardState)
{
    switch (boardState) {
    case BoardState::XWins:
        return "player X wins!";
    case BoardState::OWins:
        return "player O wins!";
    case BoardState::Tie:
        return "it's a tie!";
    default:
        return "";
    }
}

void TicTacToeGame::declareGameState(BoardState boardState)
{
    QMessageBox resultBox;
    resultBox.setWindowTitle("Game Result");
    resultBox.setText("Game over, " + getBoardFinalStateText(boardState));
    resultBox.exec();
}

void TicTacToeGame::reset(vector<Cell> &cells)
{
    // Resets the GUI cells to an empty button with no text marks.
    for (auto &cell : cells)
        cell.cellBtn->setText("");
}

void TicTacToeGame::updateConnectionStatus(bool isConnected)
{
    // Mensaje de depuración para confirmar que el slot se está ejecutando
    qDebug() << "Slot updateConnectionStatus llamado. isConnected:" << isConnected;

    // Paso 1: Limpiar explícitamente el estilo anterior.
    ui->connectionLabel->setStyleSheet(QString());

    // Acceder al LED a través del puntero de UI
    if (isConnected) {
        // Estilo VERDE (Conectado)
        ui->connectionLabel->setStyleSheet(
            "background-color: #4CAF50;" // Verde
            // border-radius = 10px (la mitad de 20px) garantiza el círculo
            "border-radius: 10px;"
            "border: 2px solid #388E3C;"
            );
        ui->connectionLabel->setToolTip("Estado de Conexión: Conectado");
    } else {
        // Estilo ROJO (Desconectado)
        ui->connectionLabel->setStyleSheet(
            "background-color: #F44336;" // Rojo
            "border-radius: 10px;"
            "border: 2px solid #D32F2F;"
            );
        ui->connectionLabel->setToolTip("Estado de Conexión: Desconectado");
    }
}

void TicTacToeGame::updateRecordingStatus(bool isRecording)
{
    // Mensaje de depuración para confirmar que el slot se está ejecutando
    qDebug() << "Slot updateRecordingStatus llamado. isRecording:" << isRecording;

    // Paso 1: Limpiar explícitamente el estilo anterior.
    ui->connectionLabel->setStyleSheet(QString());

    // Acceder al LED a través del puntero de UI
    if (isRecording) {
        // ROJO: Grabando
        setLedStyle(ui->RecordLabel, "REC", "#e74c3c", "white", 12);
    } else {
        // GRIS: No grabando
        setLedStyle(ui->RecordLabel, "REC", "#7f8c8d", "white", 12);
    }
    this->adjustSize();
}

void TicTacToeGame::updateWaitingStatus(bool isWaiting)
{
    // Mensaje de depuración para confirmar que el slot se está ejecutando
    qDebug() << "Slot updateWaitingStatus llamado. isWaiting:" << isWaiting;

    // Paso 1: Limpiar explícitamente el estilo anterior.
    ui->WaitingLabel->setStyleSheet(QString());

    // Acceder al LED a través del puntero de UI
    if (isWaiting) {
        // NARANJA/AMARILLO: Esperando respuesta del servidor
        setLedStyle(ui->WaitingLabel, "ESPERANDO", "#f39c12", "white", 9);
        waiter->start();
    } else {
        // VERDE CLARO: Listo para recibir input/acción
        setLedStyle(ui->WaitingLabel, "LISTO", "#27ae60", "white", 9);
        waiter->stop();
    }
    this->adjustSize();
}

void TicTacToeGame::setLedStyle(QLabel* label, const QString& text, const QString& bgColor, const QString& textColor, int fontSize){
    if (label) {
        label->setText(text);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet(QString(
                                 "QLabel { "
                                 "   font-weight: bold; "
                                 "   font-size: %1pt; "
                                 "   color: %2; "
                                 "   background-color: %3; "
                                 "   border: 1px solid rgba(0, 0, 0, 0.2); "
                                 "   border-radius: 20px; "
                                 "   padding: 4px; "
                                 "}"
                                 ).arg(fontSize).arg(textColor, bgColor));
    }
    this->adjustSize();
}

