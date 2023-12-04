#ifndef _KBD_CONTROLLER_H_
#define _KBD_CONTROLLER_H_
// PC键盘控制

#include "cocos2d.h"
#include "Direction.h"

using namespace cocos2d;

class Player;

enum KbdEvent {
    None,
    PageUp,
    PageDown,
    PressC,
    PressV,
    PressX,
};

using kbdEventCallback = std::function<void(KbdEvent)>;

class KbdController : public Node
{
public:
    CREATE_FUNC(KbdController)

    virtual bool init();
    void attachPlayer(Player* player) { _player = player; }

    void setEventCallback(const kbdEventCallback& cb) { _eventCallback = cb; }

private:
    std::map<cocos2d::EventKeyboard::KeyCode, Direction> _table;          // 键位方向表
    Player* _player = nullptr;

    kbdEventCallback _eventCallback = nullptr;
};

#endif // _KBD_CONTROLLER_H_
