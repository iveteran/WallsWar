#include "Common.h"
#include "Joypad2.h"
#include "PlayerTank.h"
#include "GameScene.h"

const double PI = 3.141592654;

bool Joypad2::init()
{
    if (!Layer::init())
        return false;

    // 此处是4方向
    m_type = JoystickType::KEY4;

    // 锚点在圆心
    m_wheel = Sprite::create("images/joypad/wheel.png");
    m_wheel->setScale(0.5);
    m_wheel->setPosition(50, 50);
    addChild(m_wheel);

    // 摇杆
    m_stick = Sprite::create("images/joypad/stick.png");
    m_stick->setScale(0.5);
    m_stick->setPosition(m_wheel->getPosition());
    addChild(m_stick);

    m_can_move = true;

    // 开火键
    m_attack = Sprite::create("images/joypad/attack.png");
    m_attack->setScale(0.7);
    m_attack->setPosition(getContentSize().width - 50, 50);
    addChild(m_attack);

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

// 获取以p1为圆心，p2p1与x轴正方向的弧度值
float Joypad2::calcRad(Point p1, Point p2)
{
    float xx = p2.x - p1.x;
    float yy = p2.y - p1.y;

    // 斜边
    float xie = sqrt(pow(xx, 2) + pow(yy, 2));
    // yy >= 0 弧度在于 0 到 π 之间。(0~180°)
    // yy < 0 弧度在于 π 到 2π 之间。(180°~360°)
    float rad = yy >= 0 ? (acos(xx / xie)) : (PI * 2 - acos(xx / xie));
    return rad;
}

// 得到与角度对应的半径为R的圆上一坐标点, 相对值
Vec2 Joypad2::getAnglePosition(float R, float rad)
{
    return Point(R * cos(rad), R * sin(rad));
}

void Joypad2::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
//    CCLOG("onTouchBegan");
    // 某一时刻可能是一个或两个触点
    Point point1 = touches.front()->getLocation();
    Point point2 = touches.back()->getLocation();

    // 区分左右触点，可能是同一个
    Point left_point;
    Point right_point;
    if (point1.x < point2.x)
    {
        left_point = point1;
        right_point = point2;
    }
    else
    {
        left_point = point2;
        right_point = point1;
    }

    // 左触点在圈内才能移动
    if (m_wheel->getBoundingBox().containsPoint(left_point))
        m_can_move = true;

    // 右触点开火
    if (m_attack->getBoundingBox().containsPoint(right_point))
    {
        // 开火
        // callback
        auto player1 = GET_PLAYER1();
        if (player1 != nullptr) {
            player1->shoot();
        }
    }
}

void Joypad2::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
//    CCLOG("onTouchMoved");

    if (!m_can_move)
        return;

    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    Size visible_size = Director::getInstance()->getVisibleSize();

    // 某一时刻可能是一个或两个触点
    Point point1 = touches.front()->getLocation();
    Point point2 = touches.back()->getLocation();

    Point wheel_center = m_wheel->getPosition();
    float wheel_radius = m_wheel->getContentSize().width / 2;
//    float stick_radius = m_stick->getContentSize().width / 2;

    // 区分左右触点，可能是同一个
    Point left_point;
    Point right_point;
    if (point1.x < point2.x)
    {
        left_point = point1;
        right_point = point2;
    }
    else
    {
        left_point = point2;
        right_point = point1;
    }

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
                auto player1 = GET_PLAYER1();
                if (player1 != nullptr) {
                    player1->setDirection(direction);
                    player1->playAnimate();
                    player1->startMove();
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

    // 某一时刻可能是一个或两个触点
    Point point1 = touches.front()->getLocation();
    Point point2 = touches.back()->getLocation();

    // 区分左右触点，可能是同一个
    Point left_point;
    Point right_point;
    if (point1.x < point2.x)
    {
        left_point = point1;
        right_point = point2;
    }
    else
    {
        left_point = point2;
        right_point = point1;
    }

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
            auto player1 = GET_PLAYER1();
            if (player1 != nullptr) {
                player1->stopMove();
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
        //auto player1 = GET_PLAYER1();
        //if (player1 != nullptr) {
        //    player1->shoot();
        //}
    }
}
