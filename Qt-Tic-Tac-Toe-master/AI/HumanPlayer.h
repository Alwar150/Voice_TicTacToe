#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H
#include "Player.h"
#include "SpeechManager.h"
#include <QThread>

class HumanPlayer final : public Player
    Q_OBJECT
{
public:
    explicit HumanPlayer(QObject& parent = nullptr, const Board& board_ref, BoardMarks playerMark = BoardMarks::Empty)
    virtual ~HumanPlayer() {}
    void play() override;
signals:
    void startListening();
    void stopListening();
private slots:
    void onSpeechRecognized(const QString &text);

private:
    int parseSpeechCommand(const QString &text);
    bool validateCommand(const QString &command);
    SpeechManager* stt_;
    QThread* sttThread_;
};

#endif // HUMANPLAYER_H