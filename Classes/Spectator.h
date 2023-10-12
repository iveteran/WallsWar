#pragma once

#include "cocos2d.h"

// 观众
class Spectator : public CCNode {
public:
    bool init() override { return Gamer::init(); }
    BlockType getType() const override { return BlockType::SPECTATOR; }

    void followPlayer(const Player* player);
};
