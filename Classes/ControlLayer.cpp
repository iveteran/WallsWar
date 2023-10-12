#include "ControlLayer.h"
//#include "Joypad.h"
#include "Joypad2.h"
#include "KbdController.h"
#include "ui/CocosGUI.h"

#include "ui/CocosGUI.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIWidget.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool ControlLayer::init() {
    if (!cocos2d::Layer::init()) {
        return false;
    }

    initPosition();

    //auto visible_size = Director::getInstance()->getVisibleSize();
    //_layout = Layout::create();
    //_layout->setContentSize(visible_size);
    //this->addChild(_layout);

    _addJoypad2();
    _addKbdController();

    _addInfoArea();
    _addSettinsArea();

    return true;
}

void ControlLayer::initPosition() {
    // 玩家出生地：地图左下角
    Size visible_size = Director::getInstance()->getVisibleSize();
    float x = visible_size.width - CENTER_WIDTH / 2;
    float y = visible_size.height - CENTER_HEIGHT / 2;
    this->setPosition(x, y);
}

void ControlLayer::attachPlayer(Player* player) {
    _joypad2->attachPlayer(player);
    _kbd_ctrler->attachPlayer(player);
}

//void ControlLayer::_addJoypad() {
//    _joypad = Joypad::getInstance();
//    this->addChild(_joypad);
//}

void ControlLayer::_addJoypad2() {
    _joypad2 = Joypad2::create();
    this->addChild(_joypad2);
    _joypad2->setJoystickType(JoystickType::KEY4);
    auto size = _joypad2->getContentSize();
    printf(">> joypad size: (%f, %f)\n", size.width, size.height);
    auto pos = _joypad2->getPosition();
    printf(">> joypad position: (%f, %f)\n", pos.x, pos.y);
}

void ControlLayer::_addKbdController() {
    _kbd_ctrler = KbdController::create();
    this->addChild(_kbd_ctrler);
}

void ControlLayer::_addInfoArea() {
    auto myLabel = Label::createWithSystemFont("My Label Text", "Arial", 8);
    //myLabel->setPosition(myLabel->getContentSize().width / 2.0f, _layout->getContentSize().height - myLabel->getContentSize().height / 2.0f);
    //_layout->addChild(myLabel);
    this->addChild(myLabel);
}

void ControlLayer::_addSettinsArea() {
    //auto button = Button::create("normal_image.png", "selected_image.png", "disabled_image.png");
    auto button = Button::create();
    button->setTitleText("Button Text");
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("Button 1 clicked\n");
            break;
        default:
            break;
        }
    });

    button->setPosition(Vec2(0, 10));
    this->addChild(button);
}
