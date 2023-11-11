#pragma once

#include "Gamer.h"

class NPC : public Gamer {
public:
    bool init() override { return Gamer::init(); }
    BlockType getType() const override { return BlockType::NPC; }

    virtual bool canBeDestroy() const { return false; }
};
