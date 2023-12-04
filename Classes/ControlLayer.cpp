#include "ControlLayer.h"
#include "Joypad2.h"
#include "KbdController.h"
#include "Settings.h"
#include "StatusBar.h"
#include "MessagesBox.h"
#include "ZoomOutMap.h"
#include "BlockSelector.h"
#include "TeammatesPanel.h"
#include "Player.h"
#include "TranslateText.h"

#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"

USING_NS_CC;

static bool _tmOpAdding = true;
static std::queue<int> _playerIdList;

void addDemoNotices(ControlLayer* cl) {
    cl->addNotice("hello world", NoticeLevel::WARNING);
    cl->addNotice("hi guys", NoticeLevel::INFO);
}

void addDemoTeammates(TeammatesPanel* tp) {
    for (int i=0; i< 2; i++) {
        auto testPlayer = Player::create();
        _playerIdList.push(testPlayer->id());
        tp->addTeammate(testPlayer);
    }
}

void addAndRemoveDemoTeammates(TeammatesPanel* tp) {
    if (!tp) {
        return;
    }
    if (_playerIdList.empty()) {
        _tmOpAdding = true;
    } else if (_playerIdList.size() >= 6) {
        _tmOpAdding = false;
    }
    if (_tmOpAdding) {
        auto testPlayer = Player::create();
        _playerIdList.push(testPlayer->id());
        tp->addTeammate(testPlayer);
    } else {
        int playerId = _playerIdList.front();
        _playerIdList.pop();
        tp->removeTeammate(playerId);
    }
}

bool ControlLayer::onTouchBegan(Touch* touch, Event* event) {
    //printf(">> ControlLayer touch began\n");

    addAndRemoveDemoTeammates(_teammatesPanel);

    return true;
}

bool ControlLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(ControlLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

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

    _zoomOutMap = ZoomOutMap::create();
    _layout->addChild(_zoomOutMap);

    _blockSelector = BlockSelector::create();
    _layout->addChild(_blockSelector);

    addDemoNotices(this);

    return true;
}

void ControlLayer::attachPlayer(Player* player) {
    _player = player;
    _joypad2->attachPlayer(player);
    _kbd_ctrler->attachPlayer(player);
    _zoomOutMap->attachPlayer(player);
    _blockSelector->attachPlayer(player);

    if (player->hasTeammates()) {
        _addTeammatesPanel();
        addDemoTeammates(_teammatesPanel);
    }
}

void ControlLayer::_addJoypad2() {
    _joypad2 = Joypad2::create();
    this->addChild(_joypad2);
    _joypad2->setJoystickType(JoystickType::KEY4);
}

void ControlLayer::_addKbdController() {
    _kbd_ctrler = KbdController::create();
    _kbd_ctrler->setEventCallback(CC_CALLBACK_1(ControlLayer::_onKbdEvent, this));
    //_kbd_ctrler->setEventCallback(std::bind(&ControlLayer::_onKbdEvent, this, std::placeholders::_1));  // same as above
    this->addChild(_kbd_ctrler);
}

void ControlLayer::_onKbdEvent(KbdEvent event) {
    if (_blockSelector) {
        switch (event) {
            case KbdEvent::PageUp:
                _blockSelector->scrollUp();
                break;
            case KbdEvent::PageDown:
                _blockSelector->scrollDown();
                break;
            case KbdEvent::PressC:
                _blockSelector->switchTo1X();
                break;
            case KbdEvent::PressV:
                _blockSelector->switchTo2X();
                break;
            case KbdEvent::PressX:
                _blockSelector->switchTo4X();
                break;
            default:
                break;
        }
    }
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

void ControlLayer::_addTeammatesPanel() {
    _teammatesPanel = TeammatesPanel::create();
    _teammatesPanel->initWithPlayer(_player);
    _layout->addChild(_teammatesPanel);
}
