#include "KbdController.h"
#include "Player.h"

USING_NS_CC;

bool KbdController::init()
{
    if (!Node::init())
        return false;

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
        if (_player == nullptr) {
            CCLOG("[ERROR] the keyboard controller does not attach the player");
            return;
        }
        switch (keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            _player->startMove(_table[keyCode]);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_J:
        case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
            _player->shoot();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_K:
            _player->dropBomb();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_L:
            _player->dropSmokeBomb();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_SEMICOLON:
            _player->dropFlashBomb();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_QUOTE:
            _player->dropFireBomb();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_C:
            _player->createBlock1();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_V:
            _player->createBlock2();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_X:
            _player->createBlock4();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_LESS_THAN:
        case cocos2d::EventKeyboard::KeyCode::KEY_PG_DOWN:
            _player->choiceCreatingBlockType(RRDirection::BACKWARD);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_GREATER_THAN:
        case cocos2d::EventKeyboard::KeyCode::KEY_PG_UP:
            _player->choiceCreatingBlockType(RRDirection::FORWARD);
            break;
        default:
            break;
        }

    };

    listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event*) {
        if (_player == nullptr) {
            CCLOG("[ERROR] the keyboard controller does not attach the player");
            return;
        }
        auto direction = _table[keyCode];
        switch (keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            if (_player->getDirection() == direction) {
                _player->stopMove();
            } else {
                // 如果方向不同则只调整方向
                _player->setDirection(direction);
            }
            break;
        default:
            break;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
