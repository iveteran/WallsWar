#include "cocos2d.h"
#include "ui/UILayout.h"

using namespace cocos2d::ui;

//class Joypad;
class Joypad2;
class KbdController;
class Player;

class ControlLayer : public cocos2d::Layer {
public:
    CREATE_FUNC(ControlLayer);
    bool init() override;
    void initPosition();
    void attachPlayer(Player* player);

private:
    void _addJoypad2();
    void _addKbdController();
    void _addInfoArea();
    void _addSettinsArea();

    Layout* _layout = nullptr;

    //Joypad* _joypad = nullptr;
    Joypad2* _joypad2 = nullptr;
    KbdController* _kbd_ctrler = nullptr;
};
