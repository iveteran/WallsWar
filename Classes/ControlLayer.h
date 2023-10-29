#include "cocos2d.h"

namespace cocos2d::ui {
    class Layout;
    class Text;
}
using namespace cocos2d::ui;

//class Joypad;
class Joypad2;
class KbdController;
class Player;
class Settings;

class ControlLayer : public cocos2d::Layer {
public:
    CREATE_FUNC(ControlLayer);

    virtual bool init() override;
    void attachPlayer(Player* player);

protected:
    virtual void update(float dt) override;

    void _addJoypad2();
    void _addKbdController();

    void _addInfoBar();
    void _openSettingsDailog();

private:
    //Joypad* _joypad = nullptr;
    Joypad2* _joypad2 = nullptr;
    KbdController* _kbd_ctrler = nullptr;

    Settings* _userSettingsDialog = nullptr;
    Layout* _layout = nullptr;
    Text* _fpsLabel = nullptr;

    float _frames = 0.0f;
    float _accumDt = 0.0f;
};
