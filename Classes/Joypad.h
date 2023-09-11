#ifndef _JOYPAD_H_
#define _JOYPAD_H_
// 虚拟手柄控制层

#include "cocos2d.h"

using namespace cocos2d;

class PlayerTank;

class Joypad : public Layer
{
public:
    static Joypad* getInstance();

    CREATE_FUNC(Joypad)
    virtual bool init();
    void setPlayer(const PlayerTank* player);

private:
    bool initKeyboardKeysController();
    bool initTouchButtonsController();

private:
    PlayerTank* _player1 = nullptr;

    std::map<cocos2d::EventKeyboard::KeyCode, Direction> _table;          // 键位方向表
};

#endif // _JOYPAD_H_
