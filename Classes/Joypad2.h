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

class Player;

// 虚拟手柄控制层
class Joypad2 : public Layer
{
public:
    CREATE_FUNC(Joypad2);
    virtual bool init();

    void setJoystickType(JoystickType joystick_type);
    void attachPlayer(Player* player) { _player = player; }

protected:
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);

    float calcRad(Point p1, Point p2);
    Vec2 getAnglePosition(float R, float rad);

private:
    Sprite* m_wheel = nullptr;
    Sprite* m_stick = nullptr;
    Sprite* m_attack = nullptr;

    JoystickType m_type;
    bool m_can_move;
    bool m_direction_status; // 方向键按下
    bool m_fire_status; // 开火键按下

    Player* _player = nullptr;
};

#endif /* _JOYPAD2_H_ */
