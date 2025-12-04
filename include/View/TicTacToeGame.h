#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "TTTCommonTypes.h"
#include "ui_TicTacToeGame.h"
#include "waitingspinnerwidget.h"
#include <QDialog>
#include <vector>
#include <memory>

using std::vector;

/**
 * @file TicTacToeGame.h
 * @brief Definition of the TicTacToeGame class for the game GUI
 * @author Abdelrahman Osama
 * @version 1.0
 */

/**
 * @brief The TicTacToeGame class is responsible for the GUI of a game,
 * it is responsible for building the cells of a board, which can be of
 * variable size, and a navigation button to take the user back to
 * the title screen, and a button for resetting the game and playing a
 * new one.
 *
 * @startuml
 * class TicTacToeGame
 * --
 * -ui: std::unique_ptr<Ui::TicTacToeGame>
 * -waiter: WaitingSpinnerWidget*
 * --
 * +TicTacToeGame(QWidget* parent = nullptr)
 * +updateCell(Cell&, BoardMarks): void
 * +declareGameState(BoardState): void
 * +buildCellButtons(size_t): std::vector<Cell>
 * +reset(std::vector<Cell>&): void
 * +updateConnectionStatus(bool): void
 * +updateRecordingStatus(bool): void
 * +updateWaitingStatus(bool): void
 * --
 * TicTacToeGame ..|> QDialog
 * TicTacToeGame --> WaitingSpinnerWidget
 * @enduml
 */
class TicTacToeGame final : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief TicTacToeGame constructs a view of a game.
     * @param parent is a QWidget type parent that owns this instance of the controller.
     */
    explicit TicTacToeGame(QWidget *parent = nullptr);
    /**
     * @brief updateCell sets the Text of the cell to the current player's mark.
     * @param cell: a refernece to the clicked cell.
     * @param currentPlayer: the current player's mark (X or O).
     */
    void updateCell(Cell &cell, BoardMarks currentPlayer);
    /**
     * @brief declareGameState outputs the board's state to a QMessageBox.
     * @param boardState the current board state.
     */
    void declareGameState(BoardState boardState);
    /**
     * @brief buildCellButtons builds the GUI cells as push buttons.
     * @param boardSize: number of rows or columns of a board.
     * @return a collection of cells containing push button references
     * and their corresponding row and column.
     */
    vector<Cell> buildCellButtons(size_t boardSize);
    /**
     * @brief reset clears out the text of all push button cells.
     * @param cells: a collection of cells containing push button references
     * and their corresponding row and column.
     */
    void reset(vector<Cell> &cells);

    /**
     * @brief Update de visual indicator of the conection status with socket server
     */
    void updateConnectionStatus(bool);
    void updateRecordingStatus(bool);
    void updateWaitingStatus(bool);
    /**
     * @brief Activate or deactivate the visual loading indicator
     */


private:
    /**
     * @brief ui is a reference to the ui object of the tic tac toe game.
     */
    std::unique_ptr<Ui::TicTacToeGame> ui;
    /**
     * @brief setConnections handles setting the connection to emit if cells have cliked,
     * and the navigational back button.
     */
    void setConnections();
    /**
     * @brief getPlayerText converts a board mark to a QString.
     * @param currentPlayer: the current player's mark (X or O).
     * @return a QString of the player's mark.
     */
    QString getPlayerText(BoardMarks currentPlayer);
    /**
     * @brief getPlayerStyleSheet gets a stylesheet associated with each player.
     * @param currentPlayer: the current player's mark (X or O).
     * @return a QString stylesheet of the player given in the input.
     */
    QString getPlayerStyleSheet(BoardMarks currentPlayer);
    /**
     * @brief getBoardFinalStateText converts a state to a decleration text,
     * that will be used when declaring the final state of a game.
     * @param boardState the current board state.
     * @return QString decleration text,
     * that will be used when declaring the final state of a game.
     */
    QString getBoardFinalStateText(BoardState boardState);

    void setLedStyle(QLabel* label, const QString& text, const QString& bgColor, const QString& textColor, int fontSize);
    /**
     * @brief WaitingSpinnerWidget, intended to be an animated wheel when process is waiting server response
     */
    WaitingSpinnerWidget* waiter;


signals:
    /**
     * @brief newGame is emitted when the New Game button is clicked and
     * some reset logic must be executed.
     */
    void newGame();
    /**
     * @brief goBack is emitted when the Back button is clicked and the TtitleScreen must reappear.
     */
    void goBack();
};

#endif // MAINWINDOW_H
