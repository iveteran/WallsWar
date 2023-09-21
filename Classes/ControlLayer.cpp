#include "ControlLayer.h"
//#include "Joypad.h"
#include "Joypad2.h"
#include "KbdController.h"
#include "ui/CocosGUI.h"

#include "ui/UIButton.h"
#include "ui/UIWidget.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool ControlLayer::init() {
    if (!cocos2d::Layer::init()) {
        return false;
    }

    _addJoypad2();
    _addKbdController();

    return true;
}

//void ControlLayer::_addJoypad() {
//    _joypad = Joypad::getInstance();
//    this->addChild(_joypad);
//}

void ControlLayer::_addJoypad2() {
    _joypad2 = Joypad2::create();
    this->addChild(_joypad2);
    _joypad2->setJoystickType(JoystickType::KEY4);
}

void ControlLayer::_addKbdController() {
    _kbd_ctrler = KbdController::create();
    this->addChild(_kbd_ctrler);
}
