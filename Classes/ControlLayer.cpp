#include "ControlLayer.h"
//#include "Joypad.h"
#include "Joypad2.h"
#include "KbdController.h"
#include "Settings.h"
#include "StatusBar.h"
#include "MessagesBox.h"
#include "ZoomOutMap.h"
#include "TranslateText.h"

#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"

USING_NS_CC;

void addDemoNotices(ControlLayer* cl) {
    cl->addNotice("hello world", NoticeLevel::WARNING);
    cl->addNotice("hi guys", NoticeLevel::INFO);
}

bool ControlLayer::init() {
    if (!Layer::init()) {
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
    statusBar->setOpenSettingsCallback(CC_CALLBACK_0(ControlLayer::_toggleSettingsDailog, this));
    statusBar->setOpenMessagesBoxCallback(CC_CALLBACK_0(ControlLayer::_toggleMessagesBox, this));
    _layout->addChild(statusBar);

    auto zoomOutMap = ZoomOutMap::create();
    _layout->addChild(zoomOutMap);

    addDemoNotices(this);

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

void ControlLayer::_toggleSettingsDailog() {
    if (_settingsDialog) {
        return;
    }
    _settingsDialog = Settings::create();
    _settingsDialog->setClosedCallback(CC_CALLBACK_1(ControlLayer::_onCloseSettingsDailog, this));
    addChild(_settingsDialog);
}

void ControlLayer::_onCloseSettingsDailog(PopupLayer* dialog) {
    removeChild(_settingsDialog);
    _settingsDialog = nullptr;
}

void ControlLayer::_toggleMessagesBox() {
    _makeSureMessagesBoxCreated();
    _messagesBox->setVisible(!_messagesBox->isVisible());

    if (_noticeBar) {
        _noticeBar->setVisible(!_messagesBox->isVisible());
    }
}

void ControlLayer::_onCloseMessagesBox(PopupLayer* dialog) {
    //dialog->setVisible(false);
    _messagesBox->setVisible(false);

    if (_noticeBar) {
        _noticeBar->setVisible(true);
    }
}

void ControlLayer::_makeSureMessagesBoxCreated() {
    if (!_messagesBox) {
        _messagesBox = MessagesBox::create();
        _messagesBox->setClosedCallback(CC_CALLBACK_1(ControlLayer::_onCloseMessagesBox, this));
        _messagesBox->setVisible(false);
        _layout->addChild(_messagesBox);
    }
}

void ControlLayer::addNotice(const string& msg, NoticeLevel level) {
    if (!_noticeBar) {
        _noticeBar = NoticeBar::create();
        _noticeBar->setRemovedCallback(CC_CALLBACK_1(ControlLayer::_onNoticeRemoved, this));
        _layout->addChild(_noticeBar);
    }
    _noticeBar->addNotice(msg, level);

    _makeSureMessagesBoxCreated();
    _messagesBox->addNotice(msg, level);
}

void ControlLayer::_onNoticeRemoved(const NoticeItem* noticeItem) {
    // NOTE: NoticeItem will be nullptr
    if (_noticeBar->empty()) {
        removeChild(_noticeBar);
        _noticeBar = nullptr;
    }
}
