#include "cocos2d.h"

#include "NoticeBar.h"
#include "KbdController.h"

namespace cocos2d::ui {
    class Layout;
}
using namespace cocos2d::ui;

class Joypad2;
class KbdController;
class ZoomOutMap;
class BlockSelector;
class TeammatesPanel;
class MultiBagsPanel;
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
    void _addTeammatesPanel();

    void _onKbdEvent(KbdEvent event);
    void _toggleSettingsDailog();
    void _onCloseSettingsDailog(PopupLayer* dialog);
    void _toggleMessagesBox();
    void _onCloseMessagesBox(PopupLayer* dialog);
    void _makeSureMessagesBoxCreated();
    void _onNoticeRemoved(const NoticeItem* noticeItem);

    virtual bool onTouchBegan(Touch* touch, Event* event);

private:
    Player* _player = nullptr;
    Joypad2* _joypad2 = nullptr;
    KbdController* _kbd_ctrler = nullptr;
    ZoomOutMap* _zoomOutMap = nullptr;
    BlockSelector* _blockSelector = nullptr;

    Layout* _layout = nullptr;

    Settings* _settingsDialog = nullptr;
    MessagesBox* _messagesBox = nullptr;
    NoticeBar* _noticeBar = nullptr;
    TeammatesPanel* _teammatesPanel = nullptr;
    MultiBagsPanel* _bagsPanel = nullptr;
};
