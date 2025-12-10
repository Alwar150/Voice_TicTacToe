#include "TitleScreen.h"
#include "TTTController.h"

class TicTacTocGame;

TitleScreen::TitleScreen(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TitleScreen)
{
    ui->setupUi(this);
    setConnections();
}

void TitleScreen::setConnections()
{
    connect(ui->onePlayer, SIGNAL(toggled(bool)), SLOT(updateSinglePlayer(bool)));
    connect(ui->twoPlayers, SIGNAL(toggled(bool)), SLOT(updateTwoPlayers(bool)));
    connect(ui->AIStarts, SIGNAL(toggled(bool)), SLOT(updateAIstartsGame(bool)));
    connect(ui->miniMaxDepth, SIGNAL(valueChanged(int)),
            SLOT(updateMiniMaxDepth(int)));
    connect(ui->startGame, SIGNAL(clicked()), SLOT(startGame()));
}

void TitleScreen::updateSinglePlayer(bool checked)
{
    if (checked) {
        ui->AIStarts->setEnabled(true);
        ui->miniMaxDepth->setEnabled(true);
        options_.AIopponent = true;
    }
}

void TitleScreen::updateTwoPlayers(bool checked)
{
    if (checked) {
        ui->AIStarts->setEnabled(false);
        ui->miniMaxDepth->setEnabled(false);
        options_.AIopponent = false;
    }
}

void TitleScreen::updateAIstartsGame(bool checked)
{
    if (checked)
        options_.AIstarts = true;
    else
        options_.AIstarts = false;
}

void TitleScreen::updateMiniMaxDepth(int depth)
{
    ui->miniMaxDepthValue->setText(QString::number(depth));
    options_.miniMaxDepth = static_cast<unsigned short>(depth);
}

void TitleScreen::startGame()
{
    TTTController* ttt = new TTTController(options_,this);

    ttt->startGame();

}

void TitleScreen::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
    emit exited();
}
