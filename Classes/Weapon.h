#pragma once 

#include "MovableBlock.h"

class Weapon : public MovableBlock {
public:
    bool init() override { return MovableBlock::init(); }
    BlockType getType() const override { return BlockType::WEAPON; }
};
