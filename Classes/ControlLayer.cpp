#include "ControlLayer.h"
//#include "Joypad.h"
#include "Joypad2.h"
#include "KbdController.h"
#include "Settings.h"
#include "StatusBar.h"
#include "MessagesBox.h"
#include "TranslateText.h"

#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"

USING_NS_CC;

bool ControlLayer::init() {
    if (!cocos2d::Layer::init()) {
        return false;
    }

    _layout = Layout::create();
    _layout->setContentSize(getContentSize());
    _layout->setLayoutType(Layout::Type::RELATIVE);
    this->addChild(_layout);

    _addJoypad2();
    _addKbdController();

    // NOTE: StatusBar内部可能会销毁创建的对象并重新创建，所以返回的对象不应该被其它对象引用
    auto statusBar = StatusBar::create();
    statusBar->init(StatusBar::UNEXPANDING, StatusBar::COLORFUL);
    statusBar->setOpenSettingsCallback(CC_CALLBACK_0(ControlLayer::_openSettingsDailog, this));
    statusBar->setOpenMessagesBoxCallback(CC_CALLBACK_0(ControlLayer::_openMessagesBox, this));
    _layout->addChild(statusBar);

    return true;
}

void ControlLayer::attachPlayer(Player* player) {
    //_player = player;
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

void ControlLayer::_openSettingsDailog() {
    if (_isSettingsDailogOpen) {
        return;
    }
    auto settingsDialog = Settings::create();
    settingsDialog->setClosedCallback(CC_CALLBACK_0(ControlLayer::_onSettingsDailogClosed, this));
    addChild(settingsDialog);
    _isSettingsDailogOpen = true;
}

void ControlLayer::_onSettingsDailogClosed() {
    _isSettingsDailogOpen = false;
}

void ControlLayer::_openMessagesBox() {
    if (_isMessagesBoxOpen) {
        return;
    }
    auto messagesBox = MessagesBox::create();
    messagesBox->setClosedCallback(CC_CALLBACK_0(ControlLayer::_onMessagesBoxClosed, this));
    _layout->addChild(messagesBox);
    _isMessagesBoxOpen = true;
}

void ControlLayer::_onMessagesBoxClosed() {
    _isMessagesBoxOpen = false;
}
