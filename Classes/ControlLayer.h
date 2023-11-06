#include "cocos2d.h"

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

class ControlLayer : public cocos2d::Layer {
public:
    CREATE_FUNC(ControlLayer);

    virtual bool init() override;
    void attachPlayer(Player* player);

protected:
    void _addJoypad2();
    void _addKbdController();

    void _openSettingsDailog();
    void _onSettingsDailogClosed();

private:
    //Joypad* _joypad = nullptr;
    Joypad2* _joypad2 = nullptr;
    KbdController* _kbd_ctrler = nullptr;

    Layout* _layout = nullptr;

    bool _isSettingsDailogOpen = false;
};
