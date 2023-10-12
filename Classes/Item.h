#pragma once

#include "ImmovableBlock.h"

class Item : public ImmovableBlock {
public:
    bool init() override { return ImmovableBlock::init(); }
    BlockType getType() const override { return BlockType::ITEM; }

    ImmovableBlock* makeFrom() const = 0;
};
