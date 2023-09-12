#ifndef _JOYPAD2_H_
#define _JOYPAD2_H_

#include "cocos2d.h"

using namespace cocos2d;

// 摇杆类型，四方向、八方向、任意方向
enum class JoystickType
{
    NONE,
    KEY4,
    KEY8,
    KEYANY,
    COUNT
};

enum class JoyDirection
{
    NONE,
    LEFT,
    UP,
    RIGHT,
    DOWN,
    LEFT_UP,
    RIGHT_UP,
    RIGHT_DOWN,
    LEFT_DOWN,
    COUNT
};

// use pre declare instead of include to avoid compile error
class PlayerTank;

// 虚拟手柄控制层
class Joypad2 : public Layer
{
public:
    CREATE_FUNC(Joypad2);
    virtual bool init();

    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);

    void setJoystickType(JoystickType joystick_type);
    void setPlayer(const PlayerTank* player1);

private:
    JoystickType m_type;
    float calcRad(Point p1, Point p2);
    Vec2 getAnglePosition(float R, float rad);
    Sprite* m_wheel;
    Sprite* m_stick;
    Sprite* m_attack;
    bool m_can_move;

    bool m_direction_status; // 方向键按下
    bool m_fire_status; // 开火键按下
    PlayerTank* _player1 = nullptr; // like callback
};

#endif /* _JOYPAD2_H_ */
