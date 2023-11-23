#pragma once

#include "ImmovableBlock.h"
#include "Player.h"

using cocos2d::Map;

class Player;

class Campus : public ImmovableBlock {
public:
    static constexpr float SIZE = Block::SIZE * 2;

    static void initSpriteFrameCache();
    CREATE_FUNC(Campus);

public:
    bool init() override;
    int getSize() const override { return SIZE; }
    BlockType getType() const override { return BlockType::CAMPUS; }
    virtual bool canBeDestroy() const { return false; }

    virtual std::string getSpriteFrameName() const override { return "camp_1"; }

    Map<int64_t, Player*> getPlayers() const { return _players; }
    Player* getManager() const { return _manager; }

protected:
    Map<int64_t, Player*> _players;
    Player* _manager = nullptr;
};
