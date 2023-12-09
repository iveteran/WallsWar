#include "Joypad2.h"
#include "WeaponWheel.h"
#include "Utils.h"
#include "Player.h"

const double PI = 3.141592654;

std::tuple<Point, Point>
getTouchPointPair(const std::vector<Touch*>& touches);

bool Joypad2::init()
{
    if (!Layer::init())
        return false;

    // 此处是4方向
    m_type = JoystickType::KEY4;
    m_can_move = true;

    // 锚点在圆心
    m_wheel = Sprite::create("images/joypad/wheel_alpha.png"); // alpha 70%
    m_wheel->setScale(0.5);
    m_wheel->setPosition(50, 50);
    addChild(m_wheel);

    // 摇杆
    m_stick = Sprite::create("images/joypad/stick.png");
    m_stick->setScale(0.5);
    m_stick->setPosition(m_wheel->getPosition());
    addChild(m_stick);

    // 武器发射轮
    _weaponWheel = WeaponWheel::create();
    _weaponWheel->setPosition(Vec2(getContentSize().width - 50, 50));
    addChild(_weaponWheel);

#if 0
    // 开火键
    m_attack = Sprite::create("images/joypad/attack_alpha.png"); // alpha 70%
    m_attack->setScale(0.65);
    m_attack->setPosition(getContentSize().width - 50, 50);
    addChild(m_attack);
#endif

#if 0
    // 开火键
    auto _attack = Button::create("images/joypad/shoot.png");
    _attack->setScale(0.3);
    _attack->setPosition(Vec2(getContentSize().width - 50, 50));
    addChild(_attack);
    _attack->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            if (_player != nullptr) {
                _player->shoot();
            } else {
                CCLOG("[WARNING] the joypad does not attach player");
            }
            break;
        default:
            break;
        }
    });
#endif

    // 触摸监听,多点触控
    auto touch_listener = EventListenerTouchAllAtOnce::create();
    touch_listener->onTouchesBegan = CC_CALLBACK_2(Joypad2::onTouchesBegan, this);
    touch_listener->onTouchesMoved = CC_CALLBACK_2(Joypad2::onTouchesMoved, this);
    touch_listener->onTouchesEnded = CC_CALLBACK_2(Joypad2::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);

    return true;
}

void Joypad2::setJoystickType(JoystickType joystick_type)
{
    m_type = joystick_type;
}

void Joypad2::attachPlayer(Player* player) {
    _player = player;
    _weaponWheel->attachPlayer(player);
}

void Joypad2::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
//    CCLOG("onTouchBegan");
    // 某一时刻可能是一个或两个触点
    Point left_point, right_point;
    std::tie(left_point, right_point) = getTouchPointPair(touches);

    // 左触点在圈内才能移动
    if (m_wheel->getBoundingBox().containsPoint(left_point))
        m_can_move = true;

#if 0
    // 右触点开火
    if (m_attack->getBoundingBox().containsPoint(right_point))
    {
        // 开火
        // callback
        if (_player != nullptr) {
            _player->shoot();
        } else {
            CCLOG("[WARNING] the joypad does not attach player");
        }
    }
#endif
}

void Joypad2::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
//    CCLOG("onTouchMoved");

    if (!m_can_move)
        return;

    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    Size visible_size = Director::getInstance()->getVisibleSize();

    // 区分左右触点，可能是同一个
    Point left_point, right_point;
    std::tie(left_point, right_point) = getTouchPointPair(touches);

    Point wheel_center = m_wheel->getPosition();
    float wheel_radius = m_wheel->getContentSize().width / 2;

    // 只有左触点在左半边屏才判断摇杆
    if (left_point.x < visible_origin.x + visible_size.width / 2)
    {
        Point point = left_point;
        // 判断两个圆心的距离是否大于外圈半径
        float distance = sqrt(pow(point.x - wheel_center.x, 2) + pow(point.y - wheel_center.y, 2));

        float rad = calcRad(wheel_center, point);
        if (distance >= wheel_radius)
        {
            // 摇杆中心不超出外圈范围
            m_stick->setPosition(wheel_center + getAnglePosition(wheel_radius, rad));
        }
        else
        {
            m_stick->setPosition(point); // 摇杆跟随触点
        }

        // 换算成角度，根据键类型确定方向，将方向控制信号传给游戏场景
        float angle = rad * 180.0 / PI;
        if (m_type == JoystickType::KEY4)
        {
            // 加入控制死区，只有圆心偏移距离够长才换方向
            Direction direction = Direction::NONE;
            if (distance >= wheel_radius / 5)
            {
                // 靠近轴90度范围
                if ((angle >= 0 && angle < 45) || (angle >= 315 && angle < 360))
                    direction = Direction::RIGHT;  // 右
                else if (angle >= 45 && angle < 135)
                    direction = Direction::UP;    // 上
                else if (angle >= 135 && angle < 225)
                    direction = Direction::LEFT;  // 左
                else if (angle >= 225 && angle < 315)
                    direction = Direction::DOWN;  // 下
            }

            if (direction != Direction::NONE)
            {
                if (_player != nullptr) {
                    _player->startMove(direction);
                } else {
                    CCLOG("[WARNING] the joypad does not attach player");
                }

            }
        }
        else if (m_type == JoystickType::KEY8)
        {
            // does not supported
        }
        else if (m_type == JoystickType::KEYANY)
        {
            // does not supported
        }
    }
}

void Joypad2::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
//    CCLOG("onTouchEnded");

    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    Size visible_size = Director::getInstance()->getVisibleSize();

    // 区分左右触点，可能是同一个
    Point left_point, right_point;
    std::tie(left_point, right_point) = getTouchPointPair(touches);

    // 左触点松开
    if (left_point.x < visible_origin.x + visible_size.width / 2)
    {
        // 恢复到圆心
        Point wheel_center = m_wheel->getPosition();
        m_stick->stopAllActions();
        m_stick->runAction(MoveTo::create(0.08, wheel_center));

        m_can_move = false;

        // 停止对游戏场景的控制
        // callback
        if (m_type == JoystickType::KEY4 || m_type == JoystickType::KEY8)
        {
            if (_player != nullptr) {
                _player->stopMove();
            } else {
                CCLOG("[WARNING] the joypad does not attach player");
            }
        }
        else if (m_type == JoystickType::KEYANY)
        {
            // does not supported
        }
    }

    // 右触点松开
    if (right_point.x > visible_origin.x + visible_size.width / 2)
    {
        // 停止开火
        // callback
        //if (_player != nullptr) {
        //    _player->shoot();
        //} else {
        //    CCLOG("[WARNING] the joypad does not attach player");
        //}
    }
}

std::tuple<Point, Point>
getTouchPointPair(const std::vector<Touch*>& touches) {

    // 某一时刻可能是一个或两个触点
    Point point1 = touches.front()->getLocation();
    Point point2 = touches.back()->getLocation();

    // 区分左右触点，可能是同一个
    Point left_point, right_point;
    if (point1.x < point2.x) {
        left_point = point1;
        right_point = point2;
    } else {
        left_point = point2;
        right_point = point1;
    }

    return std::make_tuple(left_point, right_point);
}
