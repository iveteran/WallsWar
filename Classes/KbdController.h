#ifndef _KBD_CONTROLLER_H_
#define _KBD_CONTROLLER_H_
// PC键盘控制

#include "cocos2d.h"
#include "Direction.h"

using namespace cocos2d;

class Player;

class KbdController : public Node
{
public:
    CREATE_FUNC(KbdController)

    virtual bool init();
    void attachPlayer(Player* player) { _player = player; }

private:
    std::map<cocos2d::EventKeyboard::KeyCode, Direction> _table;          // 键位方向表
    Player* _player = nullptr;
};

#endif // _KBD_CONTROLLER_H_
