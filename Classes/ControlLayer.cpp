#include "ControlLayer.h"
//#include "Joypad.h"
#include "Joypad2.h"
#include "KbdController.h"
#include "Settings.h"
#include "TranslateText.h"

#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"

USING_NS_CC;
using namespace cocos2d::ui;

const float STATS_INTERVAL = 0.5;
const char* defaultFont = "Arial";
const int defaultFontSize = 12;

bool ControlLayer::init() {
    if (!cocos2d::Layer::init()) {
        return false;
    }

    auto size = getContentSize();
    printf(">> ControlLayer size: (%f, %f)\n", size.width, size.height);
    auto pos = getPosition();
    printf(">> ControlLayer position: (%f, %f)\n", pos.x, pos.y);

    //initPosition();

    //auto visible_size = Director::getInstance()->getVisibleSize();
    _layout = Layout::create();
    _layout->setContentSize(getContentSize());
    _layout->setLayoutType(Layout::Type::RELATIVE);
    this->addChild(_layout);

    _addJoypad2();
    _addKbdController();

    _addInfoBar();

    // 默认渲染更新
    scheduleUpdate();

    return true;
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
}

void ControlLayer::_addKbdController() {
    _kbd_ctrler = KbdController::create();
    this->addChild(_kbd_ctrler);
}

void ControlLayer::_addInfoBar() {
    auto barLayout = Layout::create();
    barLayout->setContentSize(Size(_layout->getContentSize().width, 10));
    barLayout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    barLayout->setBackGroundColorOpacity(200);
    barLayout->setBackGroundColor(Color3B(41, 74, 122));  // 深蓝色
    barLayout->setLayoutType(Layout::Type::RELATIVE);

    RelativeLayoutParameter* barLP = RelativeLayoutParameter::create();
    barLP->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    barLayout->setLayoutParameter(barLP);
    _layout->addChild(barLayout);

    auto myLabel = Text::create("My Label", "arial", 12);
    myLabel->setScale(0.5);  // note: 通过增大字体再缩小以提高文件清析度
    //myLabel->setAnchorPoint(Vec2(0, 0));
    RelativeLayoutParameter* labelLP = RelativeLayoutParameter::create();
    labelLP->setMargin(Margin(2, 0, 0, 0)); // 左边"留白"2像素
    labelLP->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    myLabel->setLayoutParameter(labelLP);
    myLabel->setTouchEnabled(true);
    myLabel->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            printf("My Label touched began\n");
            break;
        case Widget::TouchEventType::ENDED:
            printf("My Label touched ended\n");
            break;
        default:
            break;
        }
    });
    barLayout->addChild(myLabel);

    auto button = Button::create();
    button->setTitleText("Settings");
    button->setScale(0.5);  // NOTE: 通过增大字体再缩小以提高文件清析度
    //button->setAnchorPoint(Vec2(0, 0));
    RelativeLayoutParameter* btnLP = RelativeLayoutParameter::create();
    btnLP->setMargin(Margin(0, 0, 2, 0)); // 右边"留白"2像素
    btnLP->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    button->setLayoutParameter(btnLP);
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("Settings button clicked\n");
            _openSettingsDailog();
            break;
        default:
            break;
        }
    });
    barLayout->addChild(button);

    _fpsLabel = Text::create("0.0", "Arial", 12);
    _fpsLabel->setScale(0.5);  // NOTE: 通过增大字体再缩小以提高文件清析度
    RelativeLayoutParameter* label2LP = RelativeLayoutParameter::create();
    label2LP->setAlign(RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    _fpsLabel->setLayoutParameter(label2LP);
    barLayout->addChild(_fpsLabel);
}

void ControlLayer::update(float dt) {
    ++_frames;
    _accumDt += dt;

    if (_accumDt > STATS_INTERVAL) {
        float fps = _frames / _accumDt;
        char buf[30] = {0};
        snprintf(buf, sizeof(buf), "FPS: %0.2f", fps);
        _fpsLabel->setString(buf);
    }
}

void ControlLayer::_openSettingsDailog() {
    _userSettingsDialog = Settings::create();
    _userSettingsDialog->init(300, 200, Color3B(51, 51, 51), 128);
    addChild(_userSettingsDialog);
}
