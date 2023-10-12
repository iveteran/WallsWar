#pragma once

#include "Campus.h"

class Camp : public Campus {
public:
    static void initSpriteFrameCache();
    CREATE_FUNC(Camp);

public:
    bool init() override;
    BlockType getType() const override { return BlockType::CAMP; }

    virtual std::string getSpriteFrameName() const override { return "camp_0"; }

    Player* addPlayer1();
    Player* getPlayer1();
    Camp* getEnemyCamp() const { return _enemyCamp; }
    void setEnemyCamp(Camp* camp) {
        if (camp == nullptr || _enemyCamp == camp) {
            return;
        }
        _enemyCamp = camp;
        camp->setEnemyCamp(this);
    }
    int getRemainPlayers() const { return _remainPlayers; }

    void addPlayers();
    Player* addPlayer();
    void removePlayer(Player* player);

    void enableAI();

    void enableAutoAddPlayers(bool enable=true);
    void enableAutoControlPlayers(bool enable=true);

    void autoAddPlayers(float);
    void autoControlPlayersDirection(float);
    void autoControlPlayersShoot(float);

    virtual void onBeCollided(Block* activeBlock) override;

    bool isLost() const;
    bool isWin() const;
    void showLostAnimate();

    void GameOver();
    bool isCampOk = true;                           // 大本营是否完好

private:
    //int _remainPlayers = ENEMIES_COUNT;       // 剩余未出生的敌方坦克
    int _remainPlayers = 6;
    Camp* _enemyCamp = nullptr;
};
