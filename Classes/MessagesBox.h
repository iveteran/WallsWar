#ifndef _MESSAGES_BOX_H_
#define _MESSAGES_BOX_H_

#include "ui/UIText.h"
#include "PopupLayer.h"

using std::string;
using std::vector;

USING_NS_CC;

namespace cocos2d::ui {
}
using namespace cocos2d::ui;

class Player;

enum class BoxSize {
    NONE,
    SMALL,
    NORMAL,
    LARGE,
    HUGE,
};

enum class NoticeType {
    NONE,
    INFO,
    NOTICE,
    WARNING,
    ERROR,
};

struct MessageItem : public Ref {
    int64_t     id;
    int64_t     playerId;
    string      playerName;
    time_t      time;
    string      content;
    bool        isVoice;
    bool        isOwn;
    vector<int64_t> reminderPlayers;
    NoticeType      noticeType;
};

class MessageBubble : public Text {
public:
    CREATE_FUNC(MessageBubble);
    virtual bool init() override;

    void setBackgroundColor(const Color3B& color);
    void setMessageItem(const MessageItem* msgItem);

    void playVoice();
    void cancelPlaying();
    void convertVoiceToText();

protected:
    void showSenderAvatar();
    void showContent();
    void showTime();

private:
    MessageItem* _msgItem;
};

class MessagesBox : public PopupLayer {

public:
    CREATE_FUNC(MessagesBox);

    virtual bool init() override;
    bool init(BoxSize sizeType, const Color3B& bgColor, int bgOpacity);
    bool init(int width, int height, const Color3B& bgColor, int bgOpacity);

    Size getCustomizedSize(BoxSize sizeType);
    void changeSize(BoxSize sizeType);
    void turnOn();
    void turnOff();
    void close();
    void expandSettingsPanel();

    void enableAutoPlayVoice(bool enable);
    //void playVoice(int msgId);
    void playUnreadVoiceMessaes();

    void enableAutoConvertVoiceToText(bool enable);
    //void convertVoiceToText(int msgId);
    void convertUnreadVoicesToText();

    void addMessage(const Player* player, string& message);
    void addSystemMessage(NoticeType type, string& message);
    void loadHistory(time_t since, int count);

private:
    Vector<MessageBubble*> _messageItems;
};

#endif // _MESSAGES_BOX_H_
