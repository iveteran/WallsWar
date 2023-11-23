#pragma once

#include "MovableBlock.h"

class Camp;

class Gamer : public MovableBlock {
public:
    static constexpr float SIZE = Block::SIZE * 2;

public:
    bool init() override { return MovableBlock::init(); }
    int getSize() const override { return SIZE; }
    BlockType getType() const override { return BlockType::GAMER; }

    void setName(const std::string& name) { _name = name; };

    bool beControlled() { return _beControlled; }
    int getScore() const { return _score; }

    bool isCameraFollow() const { return _cameraFollow; }
    void setCameraFollow() { _cameraFollow = true; }

    virtual Camp* getCamp() const { return nullptr; }

protected:
    std::string _name;
    bool _beControlled = false;
    int _score = 0;
    int _level = 0;
    bool _cameraFollow = false;
};
