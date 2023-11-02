#include "StatusBar.h"
#include "Camp.h"
#include "Player.h"

#include "ui/CocosGUI.h"
#include "ui/UILayout.h"
#include "ToggleButton.h"

static const float STATS_INTERVAL = 1.0;
static const char* defaultFont = "fonts/simhei.ttf";
static int defaultFontSize = 12;
static float defaultFontScale = 0.5f;
static int defaultTitleHeight = 10;
static int _buttonWidth = 10;
static const Color3B defaultBgColor = Color3B(41, 74, 122);  // 深蓝色
static int defaultOpacity = 128;

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
    setLayoutParameter(lp);

    buildStatusGroups();

    // 默认渲染更新
    scheduleUpdate();

    return true;
}

void StatusBar::setOpenSettingsCallback(const openSettingsCallback& callback) {
    _openSettingsCallback = callback;
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

    auto parent = getParent();
    removeFromParent();
    parent->addChild(statusBar);
}

Layout* StatusBar::createInfoGroupLayout(const Color3B& bgColor,
        LinearLayoutParameter::LinearGravity alignMethod,
        const ccWidgetTouchCallback& callback) {

    auto layout = Layout::create();
    float infoGroupWidth, infoGroupHeight;
    if (_expandingMode) {
        infoGroupWidth = getContentSize().width - _buttonWidth * 2;
        infoGroupHeight = getContentSize().height / _numInfoGroups;
    } else {
        infoGroupWidth = (getContentSize().width - _numButtons * _buttonWidth) / _numInfoGroups;
        infoGroupHeight = getContentSize().height;
    }
    layout->setContentSize(Size(infoGroupWidth, infoGroupHeight));
    layout->setLayoutType(Layout::Type::HORIZONTAL);
    if (_useColorful) {
        layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        layout->setBackGroundColor(bgColor);
        layout->setBackGroundColorOpacity(defaultOpacity);
    }
    if (callback) {
        layout->setTouchEnabled(true);
        layout->addTouchEventListener(callback);
    }

    auto lp = LinearLayoutParameter::create();
    lp->setGravity(alignMethod);
    layout->setLayoutParameter(lp);

    return layout;
}

void StatusBar::buildGameInfoGroup(Layout* parentLayout) {
    auto layout = createInfoGroupLayout(
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
    parentLayout->addChild(layout);

    _gameInfo.fpsLabel = Text::create("FPS: N/A", defaultFont, defaultFontSize);
    _gameInfo.fpsLabel->setScale(defaultFontScale);  // note: 通过增大字体再缩小以提高文件清析度
    auto lp = LinearLayoutParameter::create();
    lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    lp->setMargin(Margin(2, 0, 0, 0)); // 左边"留白"2像素
    _gameInfo.fpsLabel->setLayoutParameter(lp);
    layout->addChild(_gameInfo.fpsLabel);
}

void StatusBar::buildCampsInfoGroup(Layout* parentLayout) {
    auto layout = createInfoGroupLayout(
        //Color3B::BLUE,
        Color3B(0, 175, 255),
        //Color3B(114, 159, 207),
        LinearLayoutParameter::LinearGravity::CENTER_VERTICAL,
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
    parentLayout->addChild(layout);

    auto myLabel = Text::create("camps: n/a", defaultFont, defaultFontSize);
    myLabel->setScale(defaultFontScale);  // note: 通过增大字体再缩小以提高文件清析度
    auto lp = LinearLayoutParameter::create();
    lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    lp->setMargin(Margin(2, 0, 0, 0)); // 左边"留白"2像素
    myLabel->setLayoutParameter(lp);
    layout->addChild(myLabel);
}

void StatusBar::buildPlayerInfoGroup(Layout* parentLayout) {
    auto layout = createInfoGroupLayout(
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
    parentLayout->addChild(layout);

    auto nameLabel = Text::create("username: n/a", defaultFont, defaultFontSize);
    nameLabel->setScale(defaultFontScale);  // NOTE: 通过增大字体再缩小以提高文件清析度
    auto lp = LinearLayoutParameter::create();
    lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    lp->setMargin(Margin(2, 0, 0, 0)); // 左边"留白"2像素
    nameLabel->setLayoutParameter(lp);
    layout->addChild(nameLabel);
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

    createServerPlayingButton(layout);
    createServerSettingsButton(layout);
    createSettingsButton(layout);
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
}

void StatusBar::startServerPlaying(const ToggleButton* sender) {
    printf("StatusBar::startServerPlaying called\n");
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

void StatusBar::update(float dt) {
    ++_gameInfo.frames;
    _accumDt += dt;

    if (_accumDt > STATS_INTERVAL) {
        float fps = _gameInfo.frames / _accumDt;
        char buf[30] = {0};
        snprintf(buf, sizeof(buf), "  FPS: %0.2f", fps);
        _gameInfo.fpsLabel->setString(buf);

        _accumDt = 0;
        _gameInfo.frames = 0;
    }
}
