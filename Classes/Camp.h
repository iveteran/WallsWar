#pragma once

#include "Campus.h"

//constexpr unsigned char ENEMIES_COUNT = 20;
constexpr unsigned char ENEMIES_COUNT = 6;

using std::string;

class ActorController;
class CampAI;

class Camp : public Campus {
public:
    static constexpr float SIZE = Campus::SIZE;
    static const constexpr char* UIIcon = "images/block-icon-camp.png";

    static void initSpriteFrameCache();
    static Camp* create(const char* name, const Vec2& pos);

public:
    BlockType getType() const override { return BlockType::CAMP; }
    const string& getName() const { return _name; }

    virtual std::string getSpriteFrameName() const override { return "camp_0"; }

    Player* addHost();

    const Map<int, Camp*>& getEnemyCamps() const;
    void addEnemyCamp(Camp* camp);

    int getRemainPlayers() const { return _remainPlayers; }
    bool requestLifeOk();

    Player* addPlayer(ActorController* controller=nullptr);
    void addPlayer(Player* player);
    void removePlayer(Player* player);
    void removePlayer(int playerId);

    void enableAI();

    virtual void onBeCollided(Block* activeBlock) override;

    bool isLost() const;
    bool isWin() const;
    void showLostAnimate();

protected:
    bool init(const char* name, const Vec2& pos);

private:
    string _name;
    int _remainPlayers = ENEMIES_COUNT;       // 剩余未出生的敌方坦克
    Map<int, Camp*> _enemyCamps;
    bool _joinedBattle = false;

    CampAI* _ai = nullptr;
    bool isCampOk = true;                           // 大本营是否完好
};
