#pragma once

#include "cocos2d.h"
#include "Actor.h"

class Player;

// 观众
class Spectator : public Actor {
public:
    bool init() override { return Actor::init(); }
    BlockType getType() const override { return BlockType::SPECTATOR; }

    void followPlayer(const Player* player);
};
