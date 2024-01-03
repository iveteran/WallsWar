#pragma once

#include "ActorController.h"

class Player;
class Npc;
class Camp;

class AI : public ActorController {
public:
    CREATE_FUNC(AI);
    virtual bool init() { return ActorController::init(); }

    virtual Type getType() const { return ActorController::AI; }

    virtual void run() {}
    virtual void stop() {}
};

class PlayerAI : public AI {
public:
    static PlayerAI* create(Player* player);
    bool init(Player* player);

    virtual void run() override;
    virtual void stop() override;

protected:
    void update(float dt);
    void enableAutoControl(bool enable=true);
    void autoControlDirection(float);
    void autoControlShoot(float);

private:
    Player* _player = nullptr;
};

class NpcAI : public AI {
public:
    static NpcAI* create(Npc* camp);

    virtual bool init() override { return AI::init(); }

private:
    Npc* _npc = nullptr;
};

class CampAI : public AI {
public:
    static CampAI* create(Camp* camp);
    bool init(Camp* camp);

    virtual void run() override;
    virtual void stop() override;

protected:
    void update(float dt);
    void autoAddPlayers(float);
    void addPlayers();
    void enableAutoAddPlayers(bool enable=true);

private:
    Camp* _camp = nullptr;
    float _accumDt = 0.f;
};
