#pragma once

#include "Actor.h"

class GameManager : public Actor {
public:
    bool init() override { return Actor::init(); }
    int getSize() const override { return Actor::SIZE; }
    BlockType getType() const override { return BlockType::MANAGER; }
    void setName(const std::string& name) { _name = name; };

    void noticePlayer(const std::string& msg, int64_t playerId);
    void noticePlayers(const std::string& msg, std::vector<int64_t> players);

protected:
    std::string _name;
};
