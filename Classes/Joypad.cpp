#include "Common.h"
#include "Joypad.h"
#include "PlayerTank.h"

USING_NS_CC;

static Joypad* _jaypad = nullptr;

Joypad* Joypad::getInstance()
{
    if (!_jaypad) {
        _jaypad = Joypad::create();
        _jaypad->retain();
    }

    return _jaypad;
}

void Joypad::setPlayer(const PlayerTank* player1)
{
    _player1 = (PlayerTank*)player1;
}


bool Joypad::init()
{
    if (!Layer::init())
        return false;

    bool success = this->initTouchButtonsController();
    if (!success) {
        return false;
    }
    // for PC
    success = this->initKeyboardKeysController();
    return success;
}

bool Joypad::initTouchButtonsController()
{
    auto btn_left = Sprite::create("images/move_left.png");
    auto btn_up = Sprite::create("images/move_up.png");
    auto btn_right = Sprite::create("images/move_right.png");
    auto btn_down = Sprite::create("images/move_down.png");
    auto btn_attack = Sprite::create("images/attack.png");
    btn_attack->setScale(0.5);

    btn_left->setPosition(15, WINDOW_HEIGHT - 180);
    btn_up->setPosition(37.5f, WINDOW_HEIGHT - 160);
    btn_right->setPosition(60, WINDOW_HEIGHT - 180);
    btn_down->setPosition(37.5f, WINDOW_HEIGHT - 200);
    btn_attack->setPosition(WINDOW_WIDTH, WINDOW_HEIGHT - 180);

    addChild(btn_left);
    addChild(btn_up);
    addChild(btn_right);
    addChild(btn_down);
    addChild(btn_attack);

    auto touchListener = EventListenerTouchOneByOne::create();

    touchListener->onTouchBegan = [=](Touch* touch, Event*) {
        auto point = touch->getLocation();
        bool isMove = false;

        if (btn_left->getBoundingBox().containsPoint(point)) {
            _player1->setDirection(Direction::LEFT);
            isMove = true;
        } else if (btn_up->getBoundingBox().containsPoint(point)) {
            _player1->setDirection(Direction::UP);
            isMove = true;
        } else if (btn_right->getBoundingBox().containsPoint(point)) {
            _player1->setDirection(Direction::RIGHT);
            isMove = true;
        } else if (btn_down->getBoundingBox().containsPoint(point)) {
            _player1->setDirection(Direction::DOWN);
            isMove = true;
        } else if (btn_attack->getBoundingBox().containsPoint(point)) {
            _player1->shoot();
        }

        if (isMove) {
            _player1->playAnimate();
            _player1->startMove();
        }

        return true;
    };

    touchListener->onTouchEnded = [=](Touch* touch, Event*) {
        auto point = touch->getLocation();
        if (btn_left->getBoundingBox().containsPoint(point)
            || btn_up->getBoundingBox().containsPoint(point)
            || btn_right->getBoundingBox().containsPoint(point)
            || btn_down->getBoundingBox().containsPoint(point)) {

            _player1->stopAnimate();
            _player1->stopMove();
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool Joypad::initKeyboardKeysController()
{
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
            if (_player1->canMove) {
                _player1->setDirection(_table[keyCode]);
                _player1->playAnimate();
                _player1->startMove();
            }
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_J:
        case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
            _player1->shoot();
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
            if (_player1->canMove) {
                _player1->stopAnimate();
                _player1->stopMove();
            }
            break;
        default:
            break;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
