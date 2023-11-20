#include "StatusBar.h"
#include "Camp.h"
#include "Player.h"
#include "Common.h"
#include "GameScene.h"
#include "ControlLayer.h"

#include "ui/CocosGUI.h"
#include "ui/UILayout.h"
#include "ToggleButton.h"

static const float STATS_INTERVAL_ONE_SECOND = 1;  // seconds
static const float STATS_INTERVAL_FEW_SECONDS = 3;  // seconds
static const char* defaultFont = "fonts/simhei.ttf";
static int defaultFontSize = 12;
static float defaultFontScale = 0.5f;
static int defaultTitleHeight = 10;
static int _buttonWidth = 10;
static const Color3B defaultBgColor = Color3B(41, 74, 122);  // 深蓝色
static int defaultOpacity = 128;
static time_t _startTime = time(NULL);
static const bool NEED_SEPERATE = true;

USING_NS_CC;

bool StatusBar::init() {
    return Layout::init();
}

bool StatusBar::init(bool expandingMode, bool colorful) {
    if (!init()) {
        return false;
    }

    _expandingMode = expandingMode;
    _useColorful = colorful;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    return init(visibleSize.width, defaultTitleHeight, defaultBgColor, defaultOpacity);
}

bool StatusBar::init(int width, int height, const Color3B& bgColor, int opacity, const char* bgImage) {
    if (!Layout::init()) {
        return false;
    }
    setName("status_bar");

    _numInfoGroups = 3;
    _numButtons = 4;
    _buttonMargin = 2.5;

    if (_expandingMode) {
        height *= _numInfoGroups;
    }

    setLayoutType(Layout::Type::RELATIVE);
    setContentSize(Size(width, height));
    if (!_useColorful) {
        setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        setBackGroundColor(bgColor);
        setBackGroundColorOpacity(opacity);
    }

    // 置于屏幕正上方
    auto lp = RelativeLayoutParameter::create();
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    lp->setRelativeName(getName()); //给组件布局属性设置一个名字，别人可以找到它
    setLayoutParameter(lp);

    buildStatusGroups();

    // 默认渲染更新
    scheduleUpdate();

    return true;
}

void StatusBar::setOpenSettingsCallback(const targetClickedCallback& callback) {
    _openSettingsCallback = callback;
}

void StatusBar::setOpenMessagesBoxCallback(const targetClickedCallback& callback) {
    _openMessagesBoxCallback = callback;
}

void StatusBar::buildStatusGroups() {
    int _numSettingButtons = 3;

    createExpandingToggleButton();

    auto infosLayout = Layout::create();
    Size infosSize(0, getContentSize().height);
    if (_expandingMode) {
        infosLayout->setLayoutType(Layout::Type::VERTICAL);
        infosSize.width = getContentSize().width - _buttonWidth * 2;
    } else {
        infosLayout->setLayoutType(Layout::Type::HORIZONTAL);
        infosSize.width = getContentSize().width - _buttonWidth * _numSettingButtons;
    }
    infosLayout->setContentSize(infosSize);
    auto lp = RelativeLayoutParameter::create();
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::LOCATION_RIGHT_OF_CENTER);
    lp->setRelativeToWidgetName("expanding_button");//设定当前组件要与哪个组件对齐
    infosLayout->setLayoutParameter(lp);
    addChild(infosLayout);

    buildGameInfoGroup(infosLayout);
    buildCampsInfoGroup(infosLayout);
    buildPlayerInfoGroup(infosLayout);

    int numButtons = 3;
    buildSettingButtonsGroup(numButtons);
}

void StatusBar::createExpandingToggleButton() {
    auto layout = Layout::create();
    layout->setContentSize(Size(_buttonWidth, getContentSize().height));
    layout->setLayoutType(Layout::Type::RELATIVE);
    if (_useColorful) {
        layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        layout->setBackGroundColor(Color3B::WHITE);
        layout->setBackGroundColorOpacity(defaultOpacity);
    }
    layout->setTouchEnabled(true);
    layout->addTouchEventListener(
        [&](Ref* sender, Widget::TouchEventType type) {
            switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                toggleExpandingStatusBar(nullptr);
                break;
            default:
                break;
            }
        }
    );
    auto lp = RelativeLayoutParameter::create();
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    lp->setRelativeName("expanding_button");
    layout->setLayoutParameter(lp);
    addChild(layout);

    auto expandingButton = ToggleButton::create(
        "images/pull-up.png", "images/pull-down.png",
        CC_CALLBACK_1(StatusBar::unexpandStatusBar, this),
        CC_CALLBACK_1(StatusBar::expandStatusBar, this),
        _expandingMode);
    auto lp2 = RelativeLayoutParameter::create();
    lp2->setAlign(RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    expandingButton->setLayoutParameter(lp2);

    layout->addChild(expandingButton);
}

void StatusBar::expandStatusBar(const ToggleButton* sender) {
    printf("StatusBar::expandStatusBar called\n");
    toggleExpandingStatusBar(sender);
}

void StatusBar::unexpandStatusBar(const ToggleButton* sender) {
    printf("StatusBar::unexpandStatusBar called\n");
    toggleExpandingStatusBar(sender);
}

void StatusBar::toggleExpandingStatusBar(const ToggleButton* sender) {
    auto statusBar = StatusBar::create();
    statusBar->init(!_expandingMode, _useColorful);
    statusBar->setOpenSettingsCallback(_openSettingsCallback);
    statusBar->setOpenMessagesBoxCallback(_openMessagesBoxCallback);

    auto parent = getParent();
    removeFromParent();
    parent->addChild(statusBar);
}

Layout* StatusBar::createInfoGroupLayout(const Color3B& bgColor,
        LinearLayoutParameter::LinearGravity alignMethod,
        const ccWidgetTouchCallback& callback) {

    auto layout = Layout::create();
    layout->setLayoutType(Layout::Type::HORIZONTAL);
    setInfoGroupSizeAndBackground(layout, bgColor);
    if (callback) {
        layout->setTouchEnabled(true);
        layout->addTouchEventListener(callback);
    }

    auto lp = LinearLayoutParameter::create();
    lp->setGravity(alignMethod);
    layout->setLayoutParameter(lp);

    return layout;
}

void StatusBar::setInfoGroupSizeAndBackground(Layout* layout, const Color3B& bgColor) {
    float infoGroupWidth, infoGroupHeight;
    if (_expandingMode) {
        infoGroupWidth = getContentSize().width - _buttonWidth * 2;
        infoGroupHeight = getContentSize().height / _numInfoGroups;
    } else {
        infoGroupWidth = (getContentSize().width - _numButtons * _buttonWidth) / _numInfoGroups;
        infoGroupHeight = getContentSize().height;
    }
    layout->setContentSize(Size(infoGroupWidth, infoGroupHeight));
    if (_useColorful) {
        layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        layout->setBackGroundColor(bgColor);
        layout->setBackGroundColorOpacity(defaultOpacity);
    }
}

void StatusBar::buildGameInfoGroup(Layout* parentLayout) {
    _gameInfo.container = createInfoGroupLayout(
        //Color3B::RED,
        Color3B(183, 117, 220),
        //Color3B(173, 127, 168),
        LinearLayoutParameter::LinearGravity::CENTER_VERTICAL,
        [&](Ref* sender, Widget::TouchEventType type) {
            switch (type) {
            case Widget::TouchEventType::BEGAN:
                printf("Game info group touched began\n");
                break;
            case Widget::TouchEventType::ENDED:
                printf("Game info group touched ended\n");
                break;
            default:
                break;
            }
        }
    );
    parentLayout->addChild(_gameInfo.container);

    updateGameInfo();
}

void StatusBar::buildCampsInfoGroup(Layout* parentLayout) {
    auto pageView = PageView::create();
    pageView->setDirection(PageView::Direction::VERTICAL);
    // optional colors: Color3B::BLUE, Color3B(114, 159, 207),
    setInfoGroupSizeAndBackground(pageView, Color3B(0, 175, 255));

    parentLayout->addChild(pageView);
    _campsInfo.container = pageView;

    buildCampsInfoViewPages(pageView);

    updateCampsInfo();
}

void StatusBar::buildPlayerInfoGroup(Layout* parentLayout) {
    _playerInfo.container = createInfoGroupLayout(
        //Color3B::GREEN,
        Color3B(135, 255, 175),
        LinearLayoutParameter::LinearGravity::CENTER_VERTICAL,
        [&](Ref* sender, Widget::TouchEventType type) {
            switch (type) {
            case Widget::TouchEventType::BEGAN:
                printf("Player info group touched began\n");
                break;
            case Widget::TouchEventType::ENDED:
                printf("Player info group touched ended\n");
                break;
            default:
                break;
            }
        }
    );
    parentLayout->addChild(_playerInfo.container);

    updatePlayerInfo();
}

void StatusBar::buildSettingButtonsGroup(int numButtons) {
    auto layout = Layout::create();
    float groupWidth = _buttonWidth;
    if (_expandingMode) {
        layout->setLayoutType(Layout::Type::VERTICAL);
        groupWidth = _buttonWidth;
    } else {
        layout->setLayoutType(Layout::Type::HORIZONTAL);
        groupWidth = _buttonWidth * numButtons;
    }
    layout->setContentSize(Size(groupWidth, getContentSize().height));
    if (_useColorful) {
        layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        layout->setBackGroundColor(Color3B::WHITE);
        layout->setBackGroundColorOpacity(defaultOpacity);
    }
    auto lp = RelativeLayoutParameter::create();
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    layout->setLayoutParameter(lp);
    addChild(layout);

    createMessagesButton(layout);
    createServerPlayingButton(layout);
    //createServerSettingsButton(layout);
    createSettingsButton(layout);
}

void StatusBar::createMessagesButton(Layout* parentLayout) {
    auto button = Button::create("images/messages.png");
    button->setScale(defaultFontScale);
    auto lp = LinearLayoutParameter::create();
    float margin = _buttonMargin;
    lp->setMargin(Margin(margin, margin, margin, margin));
    button->setLayoutParameter(lp);
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("Game messagesBox button clicked\n");
            _openMessagesBoxCallback();
            break;
        default:
            break;
        }
    });
    parentLayout->addChild(button);
}

void StatusBar::createServerPlayingButton(Layout* parentLayout) {
    auto toggleButton = ToggleButton::create(
        "images/server-playing.png", "images/server-pause.png",
        CC_CALLBACK_1(StatusBar::startServerPlaying, this),
        CC_CALLBACK_1(StatusBar::pauseServerPlaying, this));

    auto lp = LinearLayoutParameter::create();
    float margin = _buttonMargin;
    lp->setMargin(Margin(margin, margin, margin, margin));
    //lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    toggleButton->setLayoutParameter(lp);

    parentLayout->addChild(toggleButton);
}

void StatusBar::pauseServerPlaying(const ToggleButton* sender) {
    printf("StatusBar::pauseServerPlaying called\n");
    GET_CONTROL_LAYER()->addNotice("Game server paused", NoticeLevel::WARNING);
}

void StatusBar::startServerPlaying(const ToggleButton* sender) {
    printf("StatusBar::startServerPlaying called\n");
    GET_CONTROL_LAYER()->addNotice("Game server started or resumed", NoticeLevel::WARNING);
}

void StatusBar::createServerSettingsButton(Layout* parentLayout) {
    auto button = Button::create("images/game-server.png");
    button->setScale(defaultFontScale);  // NOTE: 通过增大字体再缩小以提高文件清析度
    auto lp = LinearLayoutParameter::create();
    float margin = _buttonMargin;
    lp->setMargin(Margin(margin, margin, margin, margin));
    //lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    button->setLayoutParameter(lp);
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("Game server settings button clicked\n");
            break;
        default:
            break;
        }
    });
    parentLayout->addChild(button);
}

void StatusBar::createSettingsButton(Layout* parentLayout) {
    auto button = Button::create("images/settings.png");
    button->setScale(defaultFontScale);  // NOTE: 通过增大字体再缩小以提高文件清析度
    auto lp = LinearLayoutParameter::create();
    float margin = _buttonMargin;
    lp->setMargin(Margin(margin, margin, margin, margin));
    //lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    button->setLayoutParameter(lp);
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("Settings button clicked\n");
            _openSettingsCallback();
            break;
        default:
            break;
        }
    });
    parentLayout->addChild(button);
}

Text* StatusBar::createCustomizedLabel(const char* tag, const char* value, Layout* container) {
    std::string title;
    if (tag) {
        title += tag;
        title += ": ";
    }
    if (!value) {
        value = "N/A";
    }
    title += value;

    auto label = Text::create(title, defaultFont, defaultFontSize);
    label->setScale(defaultFontScale);
    auto lp = LinearLayoutParameter::create();
    lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    lp->setMargin(Margin(2, 0, 0, 0));
    label->setLayoutParameter(lp);
    container->addChild(label);
    return label;
}

ImageView* StatusBar::createCustomizedIcon(const char* iconImg, Layout* container) {
    auto icon = ImageView::create(iconImg);
    icon->setScale(defaultFontScale);
    auto lp = LinearLayoutParameter::create();
    lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    lp->setMargin(Margin(2, 0, 0, 0));
    icon->setLayoutParameter(lp);
    container->addChild(icon);
    return icon;
}

void StatusBar::createSeperateLabel(Layout* container) {
    createCustomizedLabel(nullptr, "|", container);
}

void StatusBar::update(float dt) {
    ++_gameInfo.frames;
    _accumDt += dt;
    _accumDt2 += dt;

    if (_accumDt > STATS_INTERVAL_ONE_SECOND) {
        updateGameInfo();
        updatePlayerInfo();

        _accumDt = 0;
        _gameInfo.frames = 0;
    }

    if (_accumDt2 > STATS_INTERVAL_FEW_SECONDS) {
        updateCampsInfo();
        _accumDt2 = 0;
    }
}

void StatusBar::updateGameInfo() {
    updateUptime();
    updateFPS();
    updateNetworkLatency(_expandingMode);
    if (_expandingMode) {
        updatePlayersNumber();
        updateMapObjectNumber();
        showMapSize();
        showWindowSize(!NEED_SEPERATE);
    }
}

void StatusBar::updateUptime(bool addSep) {
    const char* tag = "UT";
    if (!_gameInfo.uptimeLabel) {
        _gameInfo.uptimeLabel = createCustomizedLabel(tag, nullptr, _gameInfo.container);
        if (addSep) {
            createSeperateLabel(_gameInfo.container);
        }
    }

    time_t now;
    time(&now);
    time_t interval = now - _startTime;
    struct tm *interval_tm = gmtime(&interval);
    if (interval_tm == NULL) {
        return;
    }
    char tmStr[30] = {0};
    if (interval_tm->tm_hour > 0) {
        strftime(tmStr, sizeof(tmStr), "%H:%M:%S", interval_tm);
    } else {
        strftime(tmStr, sizeof(tmStr), "%M:%S", interval_tm);
    }

    char buf[30] = {0};
    snprintf(buf, sizeof(buf), "%s: %s", tag, tmStr);
    _gameInfo.uptimeLabel->setString(buf);
}

void StatusBar::updateFPS(bool addSep) {
    const char* tag = "FPS";
    if (!_gameInfo.fpsLabel) {
        _gameInfo.fpsLabel = createCustomizedLabel(tag, nullptr, _gameInfo.container);
        if (addSep) {
            createSeperateLabel(_gameInfo.container);
        }
    }

    if (_gameInfo.frames > 0 &&  _accumDt > 0) {
        float fps = _gameInfo.frames / _accumDt;
        char buf[30] = {0};
        snprintf(buf, sizeof(buf), "%s: %d", tag, (int)fps);
        _gameInfo.fpsLabel->setString(buf);
    }
}

void StatusBar::updateNetworkLatency(bool addSep) {
    const char* tag = "NL";
    if (!_gameInfo.networkLatencyLabel) {
        _gameInfo.networkLatencyLabel = createCustomizedLabel(tag, nullptr, _gameInfo.container);
        if (addSep) {
            createSeperateLabel(_gameInfo.container);
        }
    }

    char buf[30] = {0};
    int latency = 100; // TODO
    const char* unit = "ms";
    snprintf(buf, sizeof(buf), "%s: %d%s", tag, latency, unit);
    _gameInfo.networkLatencyLabel->setString(buf);
}

void StatusBar::updatePlayersNumber(bool addSep) {
    const char* tag = "PN";
    if (!_gameInfo.playerNumberLabel) {
        _gameInfo.playerNumberLabel = createCustomizedLabel(tag, nullptr, _gameInfo.container);
        if (addSep) {
            createSeperateLabel(_gameInfo.container);
        }
    }

    char buf[30] = {0};
    int numPlayers = 10; // TODO
    snprintf(buf, sizeof(buf), "%s: %d", tag, numPlayers);
    _gameInfo.playerNumberLabel->setString(buf);
}

void StatusBar::updateMapObjectNumber(bool addSep) {
    const char* tag = "O";
    if (!_gameInfo.mapObjectNumberLabel) {
        _gameInfo.mapObjectNumberLabel = createCustomizedLabel(tag, nullptr, _gameInfo.container);
        if (addSep) {
            createSeperateLabel(_gameInfo.container);
        }
    }

    char buf[30] = {0};
    int numMapObjects = 1024; // TODO
    snprintf(buf, sizeof(buf), "%s: %d", tag, numMapObjects);
    _gameInfo.mapObjectNumberLabel->setString(buf);
}

void StatusBar::showMapSize(bool addSep) {
    const char* tag = "MS";
    if (!_gameInfo.mapSizeLabel) {
        _gameInfo.mapSizeLabel = createCustomizedLabel(tag, nullptr, _gameInfo.container);
        if (addSep) {
            createSeperateLabel(_gameInfo.container);
        }

        char buf[30] = {0};
        snprintf(buf, sizeof(buf), "%s: (%d,%d)", tag, (int)CENTER_WIDTH, (int)CENTER_HEIGHT);
        _gameInfo.mapSizeLabel->setString(buf);
    }
}

void StatusBar::showWindowSize(bool addSep) {
    const char* tag = "WS";
    if (!_gameInfo.windowSizeLabel) {
        _gameInfo.windowSizeLabel = createCustomizedLabel(tag, nullptr, _gameInfo.container);
        if (addSep) {
            createSeperateLabel(_gameInfo.container);
        }

        char buf[30] = {0};
        auto windowSize = Director::getInstance()->getWinSize();
        snprintf(buf, sizeof(buf), "%s: (%d,%d)", tag, (int)windowSize.width, (int)windowSize.height);
        _gameInfo.windowSizeLabel->setString(buf);
    }
}

void StatusBar::updatePlayerInfo() {
    updatePlayerScore();
    updatePlayerLevel();
    updatePlayerPower();
    updatePlayerDefense(_expandingMode);
    if (_expandingMode) {
        updatePlayerRebornTimes();
        updatePlayerRebornDelay();
        updatePlayerPosition(!NEED_SEPERATE);
    }
}

void StatusBar::updatePlayerScore(bool addSep) {
    const char* tag = "S";
    if (!_playerInfo.scoreLabel) {
        _playerInfo.scoreLabel = createCustomizedLabel(tag, nullptr, _playerInfo.container);
        if (addSep) {
            createSeperateLabel(_playerInfo.container);
        }
    }

    char buf[30] = {0};
    int score = 128; // TODO
    snprintf(buf, sizeof(buf), "%s: %d", tag, score);
    _playerInfo.scoreLabel->setString(buf);
}

void StatusBar::updatePlayerLevel(bool addSep) {
    const char* tag = "L";
    if (!_playerInfo.levelLabel) {
        _playerInfo.levelLabel = createCustomizedLabel(tag, nullptr, _playerInfo.container);
        if (addSep) {
            createSeperateLabel(_playerInfo.container);
        }
    }

    char buf[30] = {0};
    int level = 3; // TODO
    snprintf(buf, sizeof(buf), "%s: %d", tag, level);
    _playerInfo.levelLabel->setString(buf);
}

void StatusBar::updatePlayerPower(bool addSep) {
    const char* tag = "P";
    if (!_playerInfo.powerLabel) {
        _playerInfo.powerLabel = createCustomizedLabel(tag, nullptr, _playerInfo.container);
        if (addSep) {
            createSeperateLabel(_playerInfo.container);
        }
    }

    char buf[30] = {0};
    int power = 16; // TODO
    snprintf(buf, sizeof(buf), "%s: %d", tag, power);
    _playerInfo.powerLabel->setString(buf);
}

void StatusBar::updatePlayerDefense(bool addSep) {
    const char* tag = "D";
    if (!_playerInfo.defenseLabel) {
        _playerInfo.defenseLabel = createCustomizedLabel(tag, nullptr, _playerInfo.container);
        // 如果非expanding模式的最后一个label，不再需要分隔线
        if (addSep) {
            createSeperateLabel(_playerInfo.container);
        }
    }

    char buf[30] = {0};
    int defense = 16; // TODO
    snprintf(buf, sizeof(buf), "%s: %d", tag, defense);
    _playerInfo.defenseLabel->setString(buf);
}

void StatusBar::updatePlayerRebornTimes(bool addSep) {
    const char* tag = "RT";
    if (!_playerInfo.rebornTimesLabel) {
        _playerInfo.rebornTimesLabel = createCustomizedLabel(tag, nullptr, _playerInfo.container);
        if (addSep) {
            createSeperateLabel(_playerInfo.container);
        }
    }

    char buf[30] = {0};
    int value = 12;
    snprintf(buf, sizeof(buf), "%s: %d", tag, value);
    _playerInfo.rebornTimesLabel->setString(buf);
}

void StatusBar::updatePlayerRebornDelay(bool addSep) {
    const char* tag = "RD";
    if (!_playerInfo.rebornDelayLabel) {
        _playerInfo.rebornDelayLabel = createCustomizedLabel(tag, nullptr, _playerInfo.container);
        if (addSep) {
            createSeperateLabel(_playerInfo.container);
        }
    }

    char buf[30] = {0};
    int value = 8;
    snprintf(buf, sizeof(buf), "%s: %d", tag, value);
    _playerInfo.rebornDelayLabel->setString(buf);
}

void StatusBar::updatePlayerPosition(bool addSep) {
    const char* tag = "Po";
    if (!_playerInfo.positionLabel) {
        _playerInfo.positionLabel = createCustomizedLabel(tag, nullptr, _playerInfo.container);
        if (addSep) {
            createSeperateLabel(_playerInfo.container);
        }
    }

    char buf[30] = {0};
    Vec2 pos(8, 8); // TODO
    snprintf(buf, sizeof(buf), "%s: (%d,%d)", tag, (int)pos.x, (int)pos.y);
    _playerInfo.positionLabel->setString(buf);
}

void StatusBar::buildCampsInfoViewPages(PageView* pageView) {
    for (int i = 0; i < _campsInfo.campsTotal; ++i) {
        auto layout = Layout::create();
        layout->setLayoutType(Layout::Type::HORIZONTAL);
        layout->setContentSize(pageView->getContentSize());

        layout->setTouchEnabled(true);
        layout->addTouchEventListener(
            [&](Ref* sender, Widget::TouchEventType type) {
                switch (type) { case Widget::TouchEventType::BEGAN:
                    printf("Camps info group touched began\n");
                    break;
                case Widget::TouchEventType::ENDED:
                    printf("Camps info group touched ended\n");
                    break;
                default:
                    break;
                }
            }
        );
        //layout->setUserData(i);
        pageView->insertCustomItem(layout, i);

        CampInfo campInfo;
        campInfo.index = i;
        campInfo.name = std::string("队名") + std::to_string(i+1);
        campInfo.container = layout;
        _campsInfo.items.push_back(campInfo);
    }
}


void StatusBar::updateCampsInfo() {
    _campsInfo.currIndex = (_campsInfo.currIndex + 1) % _campsInfo.campsTotal;
    int index = _campsInfo.currIndex;
    _campsInfo.container->scrollToItem(index);
    auto campInfo = &(_campsInfo.items[index]);

    showCampMark(*campInfo, !NEED_SEPERATE);
    showCampIndexAndName(*campInfo);
    updateCampScore(*campInfo);
    updateCampPlayers(*campInfo);
    updateCampMemberRebornTimes(*campInfo, _expandingMode);
    if (_expandingMode) {
        updateCampPosition(*campInfo, !NEED_SEPERATE);
    }
}

void StatusBar::showCampMark(CampInfo& campInfo, bool addSep) {
    // optional:
    //   star: self camp
    //   triangle: enemy camp
    //   circle: friends camp
    //   asterisk: unknown camp
    if (!campInfo.campMarkIcon) {
        if (campInfo.index == 0) {
            campInfo.campMarkIcon = createCustomizedIcon("images/camp-mark-star.png", campInfo.container);
        } else if (campInfo.index == 1) {
            campInfo.campMarkIcon = createCustomizedIcon("images/camp-mark-triangle.png", campInfo.container);
        } else if (campInfo.index == 2) {
            campInfo.campMarkIcon = createCustomizedIcon("images/camp-mark-circle.png", campInfo.container);
        } else {
            campInfo.campMarkIcon = createCustomizedIcon("images/camp-mark-asterisk.png", campInfo.container);
        }
        if (addSep) {
            createSeperateLabel(campInfo.container);
        }
    }
}

void StatusBar::showCampIndexAndName(CampInfo& campInfo, bool addSep) {
    if (!campInfo.campIndexLabel) {
        char buf[20] = {0};
        snprintf(buf, sizeof(buf), "[%d/%s]", campInfo.index+1, campInfo.name.c_str());
        campInfo.campIndexLabel = createCustomizedLabel(nullptr, buf, campInfo.container);
        if (addSep) {
            createSeperateLabel(campInfo.container);
        }
    }
}

void StatusBar::updateCampScore(CampInfo& campInfo, bool addSep) {
    const char* tag = "S";
    if (!campInfo.scoreLabel) {
        campInfo.scoreLabel = createCustomizedLabel(tag, nullptr, campInfo.container);
        if (addSep) {
            createSeperateLabel(campInfo.container);
        }
    }

    char buf[30] = {0};
    int score = 128; // TODO
    snprintf(buf, sizeof(buf), "%s: %d", tag, score);
    campInfo.scoreLabel->setString(buf);
}

void StatusBar::updateCampPlayers(CampInfo& campInfo, bool addSep) {
    const char* tag = "P";
    if (!campInfo.playerNumberLabel) {
        campInfo.playerNumberLabel = createCustomizedLabel(tag, nullptr, campInfo.container);
        if (addSep) {
            createSeperateLabel(campInfo.container);
        }
    }

    char buf[30] = {0};
    int value = 10 * (campInfo.index + 1); // TODO: get value of camp for index
    snprintf(buf, sizeof(buf), "%s: %d", tag, value);
    campInfo.playerNumberLabel->setString(buf);
}

void StatusBar::updateCampPosition(CampInfo& campInfo, bool addSep) {
    const char* tag = "Po";
    if (!campInfo.positionLabel) {
        campInfo.positionLabel = createCustomizedLabel(tag, nullptr, campInfo.container);
        if (addSep) {
            createSeperateLabel(campInfo.container);
        }
    }

    char buf[30] = {0};
    Vec2 pos(8, 8); // TODO
    snprintf(buf, sizeof(buf), "%s: (%d,%d)", tag, (int)pos.x, (int)pos.y);
    campInfo.positionLabel->setString(buf);
}

void StatusBar::updateCampMemberRebornTimes(CampInfo& campInfo, bool addSep) {
    const char* tag = "RT";
    if (!campInfo.playersRebornTimesLabel) {
        campInfo.playersRebornTimesLabel = createCustomizedLabel(tag, nullptr, campInfo.container);
        if (addSep) {
            createSeperateLabel(campInfo.container);
        }
    }

    char buf[30] = {0};
    int value = 12;
    snprintf(buf, sizeof(buf), "%s: %d", tag, value);
    campInfo.playersRebornTimesLabel->setString(buf);
}
