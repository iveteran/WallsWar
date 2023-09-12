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
