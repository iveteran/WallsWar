#ifndef _MESSAGES_BOX_H_
#define _MESSAGES_BOX_H_

#include "PopupLayer.h"
#include "NoticeBar.h"

#include "ui/UITextField.h"
#include "ui/UIEditBox/UIEditBox.h"

using std::string;
using std::vector;

USING_NS_CC;

namespace cocos2d::ui {
    class ListView;
    class Button;
    class Text;
    //class RichText;
    class TextField;
    class EditBox;
}
using namespace cocos2d::ui;

class Player;
class ToggleButton;
class MessagesBox;
class MessageToolbar;
class MessageSearchBar;
class PopupMenu;

enum class BoxSize {
    NONE,
    SMALL,
    NORMAL,
    LARGE,
    HUGE,
};

struct MessageItem : public Ref {
    CREATE_FUNC(MessageItem);
    virtual bool init() { return true; }

    int64_t     id = 0;
    int64_t     playerId = 0;
    string      playerName;
    time_t      time;
    string      content;
    Color3B     color;
    bool        isVoice = false;
    string      voiceContent;
    bool        isOwn = false;
    vector<string>  reminderPlayers;
    NoticeLevel     level;
};

//class TimeDivider : public Text {
//};

class MutedMessagesStatsPanel : public Layout {
};

class MessageSearchBar : public Layout {
};

class MessageToolbar : public Layout {
public:
    CREATE_FUNC(MessageToolbar);
    virtual bool init() override;

    void setMessagesBox(MessagesBox* msgsBox);

    void muteMessage(const ToggleButton* sender);
    void unmuteMessage(const ToggleButton* sender);
    void clearMessages();
    void mutePlayers();

private:
    MessagesBox* _msgsBox = nullptr;

    ToggleButton* _muteToggle = nullptr;
    Button* _cleanupBtn = nullptr;
    Button* _openMutePlayersBtn = nullptr;
};

class MessageBubble : public Layout {
public:
    CREATE_FUNC(MessageBubble);
    virtual bool init() override;

    void setMessageItem(const MessageItem* msgItem);

    void playVoice();
    void cancelPlaying();
    void convertVoiceToText();

protected:
    const char* getPlayerAvatar(int64_t playerId);
    const char* getGameManagerAvatar();
    Size makeStringWrapped(const string& content, string& output);
    Size calculateBubbleSize(Size avatarSize, Size textBlockSize);

    void buildAvatar(const char* avatarImg);
    void buildContent();
    void showSenderAvatar();
    void showContent();
    void showTime();
    void showReminderPlayers();

private:
    MessageItem* _msgItem;

    Button* _avatar = nullptr;
    Text* _content = nullptr;
    //RichText* _content = nullptr;
    Text* _time = nullptr;
    Text* _playerName = nullptr;
    Text* _reminderPlayers = nullptr;

    bool _showTime = false;
};

class MessageInput : public Layout, public EditBoxDelegate {
public:
    CREATE_FUNC(MessageInput);
    virtual bool init() override;
    void setMessagesBox(MessagesBox* msgsBox);

    void input(const string& text);

protected:
    //void textFieldEvent(Ref *pSender, TextField::EventType type);

    // implement methods for EditBoxDelegate
    virtual void editBoxEditingDidBegin(EditBox* editBox) override;
    virtual void editBoxEditingDidEndWithAction(EditBox* editBox, EditBoxDelegate::EditBoxEndAction action) override;
    virtual void editBoxTextChanged(EditBox* editBox, const string& text) override;
    virtual void editBoxReturn(EditBox* editBox) override;

    void onTyping(const string& text);
    void onSent(const string& text);

    void sendInputText();
    void sendText(Ref* sender, string& text);

    void switchToKeyboardInput(const ToggleButton* sender);
    void switchToSpeechInput(const ToggleButton* sender);
    void stopSpeech(const ToggleButton* sender);
    void startSpeech(const ToggleButton* sender);

    void showToolbar(const ToggleButton* sender);
    void hideToolbar(const ToggleButton* sender);

private:
    MessagesBox* _msgsBox = nullptr;

    ToggleButton* _inputTypeBtn = nullptr;
    //TextField* _kbdInputField = nullptr;
    EditBox* _kbdInputField = nullptr;
    Button* _goBtn = nullptr;
    ToggleButton* _speechInputBtn = nullptr;
    PopupMenu* _freqStatementMenu = nullptr;

    Button* _toolbarToggle = nullptr;
};

class MessagesBox : public PopupLayer {
friend class MessageInput;

public:
    static Size getCustomizedSize(BoxSize sizeType);

public:
    CREATE_FUNC(MessagesBox);

    virtual bool init() override;
    bool init(BoxSize sizeType, const Color3B& bgColor, int bgOpacity);
    bool init(int width, int height, const Color3B& bgColor, int bgOpacity);

    void close();
    void changeSize(BoxSize sizeType);
    void mute();
    void unmute();
    void showToolbar();
    void hideToolbar();

    void enableAutoPlayVoice(bool enable);
    //void playVoice(int msgId);
    void playUnreadVoiceMessaes();

    void enableAutoConvertVoiceToText(bool enable);
    //void convertVoiceToText(int msgId);
    void convertUnreadVoicesToText();

    void addMessage(const string& message, const Player* player);
    void addSystemMessage(const string& message, NoticeLevel level);
    int addNotice(const string& message, NoticeLevel level);
    void removeNotice(int id);
    void loadHistory(time_t since, int count);
    void clearMessages();

    void attachNoticeBar(NoticeBar* noticeBar);
    void detachNoticeBar();

    void blockPlayer(int64_t playerId);

    void generateDemoMessages(int count);

protected:
    void onNoticeRemoved(const NoticeItem* noticeItem);

private:
    Vector<MessageBubble*> _pinMessageList;
    int _pinSeconds = 60;
    int _pinRollSeconds = 3;

    bool _isMute = true;
    Vector<Player*> _mutedPlayers;

    vector<string> _blockWords;

    Layout* _layout = nullptr;
    NoticeBar* _noticeBar = nullptr;
    ListView* _showMsgList = nullptr;
    MessageInput* _msgInput = nullptr;
    MessageSearchBar* _searchBar = nullptr;
    MessageToolbar* _toolbar = nullptr;
};

#endif // _MESSAGES_BOX_H_
