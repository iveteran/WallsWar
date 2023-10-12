#include "Common.h"
#include "Joypad.h"
#include "Player.h"

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

void Joypad::setPlayer(const Player* player1)
{
    _player1 = (Player*)player1;
}


bool Joypad::init()
{
    if (!Layer::init())
        return false;

    auto btn_left = Sprite::create("images/joypad/move_left.png");
    auto btn_up = Sprite::create("images/joypad/move_up.png");
    auto btn_right = Sprite::create("images/joypad/move_right.png");
    auto btn_down = Sprite::create("images/joypad/move_down.png");
    auto btn_attack = Sprite::create("images/joypad/attack.png");
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
        Direction direction = Direction::NONE;
        auto point = touch->getLocation();
        if (btn_left->getBoundingBox().containsPoint(point)) {
            direction = Direction::LEFT;
        } else if (btn_up->getBoundingBox().containsPoint(point)) {
            direction =Direction::UP;
        } else if (btn_right->getBoundingBox().containsPoint(point)) {
            direction = Direction::RIGHT;
        } else if (btn_down->getBoundingBox().containsPoint(point)) {
            direction = Direction::DOWN;
        } else if (btn_attack->getBoundingBox().containsPoint(point)) {
            _player1->shoot();
        }

        if (direction != Direction::NONE) {
            _player1->startMove(direction);
        }

        return true;
    };

    touchListener->onTouchEnded = [=](Touch* touch, Event*) {
        auto point = touch->getLocation();
        if (btn_left->getBoundingBox().containsPoint(point)
            || btn_up->getBoundingBox().containsPoint(point)
            || btn_right->getBoundingBox().containsPoint(point)
            || btn_down->getBoundingBox().containsPoint(point)) {

            _player1->stopMove();
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}
