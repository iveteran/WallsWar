#include "MenuScene.h"
#include "GameScene.h"
#include "AudioEngine.h"
#include "CCEventListener.h"

#include "constants/AppConstants.h"
#include "constants/SceneConstants.h"
#include "constants/PlayerConstants.h"

USING_NS_CC;

cocos2d::Scene* MenuScene::createScene() {
    return MenuScene::create();
}

bool MenuScene::init() {
    if (!Scene::init())
        return false;

    _initBackground();

    return true;
}

// TODO
void MenuScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    auto position = _selector->getPosition();
    static int select = 0;

    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_ENTER:
    {
        // 切换场景
        Director::getInstance()->replaceScene(GameScene::createScene());

    }
    break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_W:
        if (select == 0) {
            select = 2;
            _selector->setPosition(position.x, position.y - 2 * ARROWS_DIS);
        } else {
            select--;
            _selector->setPosition(position.x, position.y + ARROWS_DIS);
        }

        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_S:
        if (select == 2) {
            select = 0;
            _selector->setPosition(position.x, position.y + 2 * ARROWS_DIS);
        } else {
            select++;
            _selector->setPosition(position.x, position.y - ARROWS_DIS);
        }

        break;
    default:
        break;
    }
}

// TODO
bool MenuScene::onTouch(cocos2d::Touch* touch, cocos2d::Event*) {
    // 切换场景
    Director::getInstance()->replaceScene(GameScene::createScene());

    return true;
}

void MenuScene::_initBackground() {
    _offsetNode = Node::create();
    addChild(_offsetNode);
    _offsetNode->setPosition(Director::getInstance()->getVisibleSize() / 2);

    // 创建背景图片
    _background = Sprite::create("images/select_player.png");
    if (!_background) return;

    _offsetNode->addChild(_background);
    _background->getTexture()->setAliasTexParameters();
    _background->setPosition(Vec2(0, Director::getInstance()->getVisibleSize().height));

    // 按下空格键快进
    auto* keyboardListener = EventListenerKeyboard::create();

    keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event*) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_SPACE:
            _background->stopAllActions();
            _background->setPosition(Vec2(0.f, 0.f));

            if (!_isSelectorInit)
                _initSelector();
            break;
        default:
            break;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // 触摸屏幕快进
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [&](Touch* touch, Event*) {
        _background->stopAllActions();
        _background->setPosition(Vec2(0.f, 0.f));

        if (!_isSelectorInit)
            _initSelector();

        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    //背景动画
    auto bkAction = Sequence::create(
        MoveTo::create(4.f, Vec2(0, 0)),
        CallFunc::create([=]() {
        if (!_isSelectorInit)
            _initSelector();
    }),
        nullptr
        );

    _background->runAction(bkAction);
}

void MenuScene::_initSelector() {
    _selector = Sprite::create("images/m0-2-1.png");
    _selector->getTexture()->setAliasTexParameters();
    _selector->setPosition(ARROWS_X, WINDOW_HEIGHT - ARROWS_Y);

    // 循环播放动画
    auto animate = _getAnimFrames();
    _selector->runAction(RepeatForever::create(animate));

    _background->addChild(_selector);

    // 使用键盘移动光标
    auto* keyBoardlistener = EventListenerKeyboard::create();
    keyBoardlistener->onKeyPressed = CC_CALLBACK_2(MenuScene::onKeyPressed, this);

    // 触摸屏幕移动光标
    auto* touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(MenuScene::onTouch, this);

    // 监听事件
    _eventDispatcher->removeAllEventListeners();
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardlistener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    _isSelectorInit = true;
}

Animate* MenuScene::_getAnimFrames() {
    Vector<SpriteFrame*> animFrams;
    animFrams.reserve(2);

    auto a1 = SpriteFrame::create("images/m0-2-1.png", Rect(0, 0, PLAYER_SIZE, PLAYER_SIZE));
    auto a2 = SpriteFrame::create("images/m0-2-2.png", Rect(0, 0, PLAYER_SIZE, PLAYER_SIZE));
    
    a1->getTexture()->setAliasTexParameters();
    a2->getTexture()->setAliasTexParameters();

    animFrams.pushBack(a1);
    animFrams.pushBack(a2);

    auto animation = Animation::createWithSpriteFrames(animFrams, 0.1f);

    return Animate::create(animation);
}
