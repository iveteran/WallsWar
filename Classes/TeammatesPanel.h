#ifndef _TEAMMATE_PANEL_H_
#define _TEAMMATE_PANEL_H_

#include "cocos2d.h"
#include "ui/UILayout.h"

USING_NS_CC;

namespace cocos2d::ui {
    class Text;
    class ListView;
}
using namespace cocos2d::ui;

class Player;

enum class TeammateType {
    NONE,
    NORMAL,
    SELF,
    LEADER,
};

class Teammate : public Widget {
public:
    static Teammate* create(Player* player);
    bool init(Player* player);

    void setType(TeammateType type) { _type = type; }

private:
    Player* _player = nullptr;
    TeammateType _type = TeammateType::NORMAL;
    int _votes = 0;
};

class TeammatesPanel : public Layout {
friend class Teammate;
public:
    CREATE_FUNC(TeammatesPanel);
    bool initWithPlayer(Player* player);

    void addCountLabel();
    void addSelf(Player* player);
    void addTeammate(Player* player, int index=-1);
    void removeTeammate(int64_t playerId);
    void voteLeader(int64_t playerId);

    int count() const;

protected:
    void updateWidthAndCountLabel();
    void updateWidth();

private:
    Text* _countLabel = nullptr;
    Teammate* _self = nullptr;
    Teammate* _leader = nullptr;
    Map<int64_t, Teammate*> _teammates;

    ListView* _teammateViews = nullptr;
};

#endif  // _TEAMMATE_PANEL_H_
