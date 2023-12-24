#include "WelcomeScene.h"
#include "NetworkingModeMenuScene.h"

#include "ui/UILoadingBar.h"
#include "ui/UIText.h"

static const int PERCENT_100 = 100;

bool WelcomeScene::init() {
    if (!Scene::init()) {
        return false;
    }

    addBackground();
    addLoadingBar();

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(WelcomeScene::onTouch, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    auto keyBoardlistener = EventListenerKeyboard::create();
    keyBoardlistener->onKeyPressed = CC_CALLBACK_2(WelcomeScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardlistener, this);

    scheduleUpdate();

    return true;
}

bool WelcomeScene::addBackground() {
    // 创建背景图片
    //_background = Sprite::create("images/select_player.png");
    //_background = Sprite::create("images/goe_welcome_480x320.png");
    //_background = Sprite::create("images/goe_welcome_960x640.png");
    _background = Sprite::create("images/goe_welcome_1280x768.png");
    if (!_background) return false;

    _background->setScale(0.4);
    _background->getTexture()->setAliasTexParameters();
    _background->setPosition(Director::getInstance()->getVisibleSize() / 2);
    addChild(_background);

    return true;
}

void WelcomeScene::addLoadingBar() {
    Size bgSize = _background->getContentSize();
    auto loadingBarPos = Vec2(bgSize.width / 2.0f, bgSize.height / 2.0f - 150);

    _loadingBar = LoadingBar::create("images/cocosui/sliderProgress.png");
    _loadingBar->setPosition(loadingBarPos);
    _background->addChild(_loadingBar);

    _elapsedLabel = Text::create();
    float margin = 10;
    _elapsedLabel->setPosition(Vec2(loadingBarPos.x, loadingBarPos.y - (_loadingBar->getContentSize().height + margin)));
    _background->addChild(_elapsedLabel);
}

void WelcomeScene::addActorAnimations() {
    // TODO
}

void WelcomeScene::update(float delta) {
    _elapsed += delta;
    _secondTick += delta;
    float loadingPercent = _loadingBar->getPercent();
    if (loadingPercent < PERCENT_100 && _secondTick >= 1.0f) {
        loadingPercent += 10;
        _loadingBar->setPercent(loadingPercent);
        _secondTick = 0.f;

        // update loading info
        auto loadingInfo = StringUtils::format("Loading %0.2f, elapsed: %0.2f", loadingPercent, _elapsed);
        _elapsedLabel->setString(loadingInfo);
    } else if (loadingPercent >= PERCENT_100) {
        gotoNextScene();
    }
}

bool WelcomeScene::onTouch(Touch* touch, Event*) {
    if (_loadingBar->getPercent() >= PERCENT_100) {
        gotoNextScene();
    }
    return true;
}

void WelcomeScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_ENTER:
    case EventKeyboard::KeyCode::KEY_SPACE:
        //if (_loadingBar->getPercent() >= PERCENT_100) {
            gotoNextScene();
        //}
    default:
        break;
    }
}

void WelcomeScene::gotoNextScene() {
    Director::getInstance()->replaceScene(NetworkingModeMenuScene::create());
}
