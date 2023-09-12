#ifndef _KBD_CONTROLLER_H_
#define _KBD_CONTROLLER_H_
// PC键盘控制

#include "cocos2d.h"

using namespace cocos2d;

class PlayerTank;

class KbdController : public Node
{
public:
    CREATE_FUNC(KbdController)
    static KbdController* getInstance();

    virtual bool init();
    void setPlayer(const PlayerTank* player);

private:
    std::map<cocos2d::EventKeyboard::KeyCode, Direction> _table;          // 键位方向表
    PlayerTank* _player1 = nullptr;
};

#endif // _KBD_CONTROLLER_H_
