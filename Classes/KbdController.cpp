#include "Common.h"
#include "KbdController.h"
#include "PlayerTank.h"
#include "GameScene.h"

USING_NS_CC;

bool KbdController::init()
{
    if (!Node::init())
        return false;

    auto player1 = GET_PLAYER1();
    if (player1 == nullptr) {
        return false;
    }

    // 初始化控制键表
    _table[EventKeyboard::KeyCode::KEY_A] = Direction::LEFT;
    _table[EventKeyboard::KeyCode::KEY_W] = Direction::UP;
    _table[EventKeyboard::KeyCode::KEY_D] = Direction::RIGHT;
    _table[EventKeyboard::KeyCode::KEY_S] = Direction::DOWN;
    _table[EventKeyboard::KeyCode::KEY_LEFT_ARROW] = Direction::LEFT;
    _table[EventKeyboard::KeyCode::KEY_UP_ARROW] = Direction::UP;
    _table[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] = Direction::RIGHT;
    _table[EventKeyboard::KeyCode::KEY_DOWN_ARROW] = Direction::DOWN;

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event*) {
        switch (keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            if (player1->canMove) {
                player1->setDirection(_table[keyCode]);
                player1->playAnimate();
                player1->startMove();
            }
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_J:
        case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
            player1->shoot();
            break;
        default:
            break;
        }

    };

    listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event*) {
        switch (keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            if (player1->canMove) {
                player1->stopAnimate();
                player1->stopMove();
            }
            break;
        default:
            break;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
