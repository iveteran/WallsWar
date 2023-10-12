#include <vector>

#include "Player.h"
#include "MapLayer.h"

USING_NS_CC;

bool Player::createBlock1(Direction dir) {
    auto playerPos = this->getPosition();
    float x = 0, y = 0;
    switch (dir) {
        case Direction::UP:
            x = playerPos.x;
            y = playerPos.y;
            break;
        case Direction::RIGHT:
            x = playerPos.x;
            y = playerPos.y - BLOCK_SIZE;
            break;
        case Direction::DOWN:
            x = playerPos.x - BLOCK_SIZE;
            y = playerPos.y - BLOCK_SIZE;
            break;
        case Direction::LEFT:
            x = playerPos.x - BLOCK_SIZE;
            y = playerPos.y;
            break;
        default:
            return false;
    }

    BlockType blockType = (BlockType)_creatingBlock;
    int blockFloor = Block::getDefaultFloor(blockType);
    bool floorChanged = false;
    if (blockFloor == getFloor()) {
        increaseFloor();
        floorChanged = true;
    }
    CCLOG(">> [Player::createBlock1] type: %d, position: (%f, %f)", blockType, x, y);
    bool success = MapLayer::getInstance()->createBlock(Vec2(x, y), blockType, this);
    CCLOG(">> [Player::createBlock1] creating success? %d", success);

    if (!success && floorChanged) {
        decreaseFloor();
    }
    return success;
}

bool Player::createBlock1() {
    bool success = false;
    int tryCount = 0;
    int directionCount = (int)Direction::COUNT;
    // 循环尝试玩家当前方向的右上、右下、左下、左上四个方向(顺时针)，直至创建成功
    for (int priorityDir = (int)_dir; tryCount < directionCount;
            priorityDir = (priorityDir + 1) % directionCount) {
        success = createBlock1((Direction)priorityDir);
        if (success) {
            break;
        }
        tryCount++;
    }
    return success;
}

bool Player::createBlock2(Direction dir) {
    auto playerPos = this->getPosition();
    std::vector<Vec2> posList;
    float x = 0, y = 0;
    switch (dir) {
        case Direction::UP:
            x = playerPos.x - BLOCK_SIZE;
            y = playerPos.y;
            posList.push_back(Vec2(x, y));
            x = playerPos.x;
            y = playerPos.y;
            posList.push_back(Vec2(x, y));
            break;
        case Direction::DOWN:
            x = playerPos.x - BLOCK_SIZE;
            y = playerPos.y - BLOCK_SIZE;
            posList.push_back(Vec2(x, y));
            x = playerPos.x;
            y = playerPos.y - BLOCK_SIZE;
            posList.push_back(Vec2(x, y));
            break;
        case Direction::LEFT:
            x = playerPos.x - BLOCK_SIZE;
            y = playerPos.y;
            posList.push_back(Vec2(x, y));
            x = playerPos.x - BLOCK_SIZE;
            y = playerPos.y - BLOCK_SIZE;
            posList.push_back(Vec2(x, y));
            break;
        case Direction::RIGHT:
            x = playerPos.x;
            y = playerPos.y;
            posList.push_back(Vec2(x, y));
            x = playerPos.x;
            y = playerPos.y - BLOCK_SIZE;
            posList.push_back(Vec2(x, y));
            break;
        default:
            return false;
    }

    BlockType blockType = (BlockType)_creatingBlock;
    int blockFloor = Block::getDefaultFloor(blockType);
    bool floorChanged = false;
    if (blockFloor == getFloor()) {
        increaseFloor();
        floorChanged = true;
    }
    CCLOG(">> [Player::createBlock2] type: %d, begin: (%f, %f), direction: %d",
            blockType, playerPos.x, playerPos.y, _dir);
    int count = MapLayer::getInstance()->createBlocks(posList, blockType, this);
    CCLOG(">> [Player::createBlock2] created %d block(s)", count);

    bool success = count > 0;
    if (!success && floorChanged) {
        decreaseFloor();
    }
    return success;
}

bool Player::createBlock2() {
    // 尝试当前方向
    bool success = createBlock2(_dir);
    if (!success) {
        // 尝试当前方向的反方向
        int reverseDir = ((int)_dir + 2) % (int)Direction::COUNT;
        success = createBlock2((Direction)(reverseDir));
    }
    return success;
}

bool Player::createBlock4() {
    auto playerPos = this->getPosition();
    std::vector<Vec2> posList;
    float x, y;
    x = playerPos.x;
    y = playerPos.y;
    posList.push_back(Vec2(x, y));
    x = playerPos.x;
    y = playerPos.y - BLOCK_SIZE;
    posList.push_back(Vec2(x, y));
    x = playerPos.x - BLOCK_SIZE;
    y = playerPos.y - BLOCK_SIZE;
    posList.push_back(Vec2(x, y));
    x = playerPos.x - BLOCK_SIZE;
    y = playerPos.y;
    posList.push_back(Vec2(x, y));

    BlockType blockType = (BlockType)_creatingBlock;
    int blockFloor = Block::getDefaultFloor(blockType);
    bool floorChanged = false;
    if (blockFloor == getFloor()) {
        increaseFloor();
        floorChanged = true;
    }
    CCLOG(">> [Player::createBlock4] type: %d, around: (%f, %f)",
            blockType, playerPos.x, playerPos.y);
    int count = MapLayer::getInstance()->createBlocks(posList, blockType, this);
    CCLOG(">> [Player::createBlock4] created %d block(s)", count);

    bool success = count > 0;
    if (!success && floorChanged) {
        decreaseFloor();
    }
    return success;
}

void Player::choiceCreatingBlockType(RRDirection rrd) {
    int beginIndex = (int)BlockType::UNDEFINED;
    int endIndex = (int)BlockType::IMMOVABLE_BLOCK;
    if (rrd == RRDirection::FORWARD) {
        _creatingBlock++;
    } else {
        _creatingBlock--;
    }
    _creatingBlock = _creatingBlock % endIndex;
    if (_creatingBlock <= beginIndex) {
        if (rrd == RRDirection::FORWARD) {
            _creatingBlock = beginIndex + 1;
        } else {
            _creatingBlock = endIndex - 1;
        }
    }
    CCLOG(">> [Player::choiceCreatingBlockType] switch to type: %d", _creatingBlock);
}
