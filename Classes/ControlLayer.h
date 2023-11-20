#include "cocos2d.h"

#include "NoticeBar.h"

namespace cocos2d::ui {
    class Layout;
}
using namespace cocos2d::ui;

//class Joypad;
class Joypad2;
class KbdController;
class Player;
class Settings;
class StatusBar;
class PopupLayer;
class MessagesBox;

class ControlLayer : public cocos2d::Layer {
public:
    CREATE_FUNC(ControlLayer);

    virtual bool init() override;
    void attachPlayer(Player* player);

    void addNotice(const string& msg, NoticeLevel level);

protected:
    void _addJoypad2();
    void _addKbdController();

    void _toggleSettingsDailog();
    void _onCloseSettingsDailog(PopupLayer* dialog);
    void _toggleMessagesBox();
    void _onCloseMessagesBox(PopupLayer* dialog);
    void _makeSureMessagesBoxCreated();
    void _onNoticeRemoved(const NoticeItem* noticeItem);

private:
    //Joypad* _joypad = nullptr;
    Joypad2* _joypad2 = nullptr;
    KbdController* _kbd_ctrler = nullptr;

    Layout* _layout = nullptr;

    Settings* _settingsDialog = nullptr;
    MessagesBox* _messagesBox = nullptr;
    NoticeBar* _noticeBar = nullptr;
};
