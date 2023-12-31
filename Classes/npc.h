#pragma once

#include "Actor.h"

class NPC : public Actor {
public:
    bool init() override { return Actor::init(); }
    BlockType getType() const override { return BlockType::NPC; }

    virtual bool canBeDestroy() const { return false; }
};
