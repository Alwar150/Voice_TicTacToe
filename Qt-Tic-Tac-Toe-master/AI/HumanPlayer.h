#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H
#include <Player.h>
#include <SpeechManager.h>
#include <QThread>

class HumanPlayer final : public Player{
    Q_OBJECT

public:
    explicit HumanPlayer(const Board& board_ref, BoardMarks playerMark = BoardMarks::Empty,QObject* parent = nullptr);
    virtual ~HumanPlayer() {}
    void play() override;
signals:
    void startListening();
    void stopListening();

private:
    void parseSpeechCommand(const QString &text);
    bool validateCommand(const QString &command);
    SpeechManager* stt_;
    QThread* sttThread_;
};

#endif // HUMANPLAYER_H
