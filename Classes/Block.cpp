#include "Block.h"
#include "ImmovableBlock.h"
#include "Player.h"
#include "NPC.h"
#include "Camp.h"
#include "MapLayer.h"

USING_NS_CC;

int64_t Block::generateID() {
    static int64_t auto_increasement_id_ = 0;
    return ++auto_increasement_id_;
}

int Block::getDefaultFloor(BlockType bt) {
    int floor = Block::DefaultFloor;
    switch (bt)
    {
        case BlockType::WALL:
        case BlockType::STONE:
        case BlockType::ICE:
        case BlockType::BRIDGE:
            floor = Block::DefaultFloor;
            break;
        case BlockType::RIVER:
        case BlockType::TRENCH:
            floor = BlockRiver::DefaultFloor;
            break;
        case BlockType::FOREST:
            floor = BlockForest::DefaultFloor;
            break;
        case BlockType::CLOUD:
            floor = BlockCloud::DefaultFloor;
            break;
        default:
            CCLOG("[getBlockFloor] unsupported block type: %d", bt);
            break;
    }
    return floor;
}

Block* Block::createBlock(BlockType type, Gamer* creator) {
    Block* block = nullptr;
    switch (type)
    {
        case BlockType::WALL:
            block = BlockWall::create();
            break;
        case BlockType::STONE:
            block = BlockStone::create();
            break;
        case BlockType::FOREST:
            block = BlockForest::create();
            break;
        case BlockType::BRIDGE:
            block = BlockBridge::create();
            break;
        case BlockType::CLOUD:
            block = BlockCloud::create();
            break;
        case BlockType::RIVER:
            block = BlockRiver::create();
            break;
        case BlockType::TRENCH:
            block = BlockTrench::create();
            break;
        case BlockType::ICE:
            block = BlockIce::create();
            break;
        default:
            CCLOG("[Block::createBlock] unsupported block type: %d", type);
            break;
    }
    if (block) {
        block->setAnchorPoint(Vec2(0, 0));
        block->setCreator(creator);
    }
    return block;
}

bool Block::isThisBlock(BlockType type, const Block* block_) {
    bool yes = false;
    Block* block = (Block*)block_;
    switch (type)
    {
        case BlockType::IMMOVABLE_BLOCK:
            yes = dynamic_cast<ImmovableBlock*>(block) != nullptr;
            break;
        case BlockType::MOVABLE_BLOCK:
            yes = dynamic_cast<MovableBlock*>(block) != nullptr;
            break;
        case BlockType::WALL:
            yes = dynamic_cast<BlockWall*>(block) != nullptr;
            break;
        case BlockType::STONE:
            yes = dynamic_cast<BlockStone*>(block) != nullptr;
            break;
        case BlockType::FOREST:
            yes = dynamic_cast<BlockForest*>(block) != nullptr;
            break;
        case BlockType::BRIDGE:
            yes = dynamic_cast<BlockBridge*>(block) != nullptr;
            break;
        case BlockType::CLOUD:
            yes = dynamic_cast<BlockCloud*>(block) != nullptr;
            break;
        case BlockType::RIVER:
            yes = dynamic_cast<BlockRiver*>(block) != nullptr;
            break;
        case BlockType::TRENCH:
            yes = dynamic_cast<BlockTrench*>(block) != nullptr;
            break;
        case BlockType::ICE:
            yes = dynamic_cast<BlockIce*>(block) != nullptr;
            break;
        case BlockType::GAMER:
            yes = dynamic_cast<Gamer*>(block) != nullptr;
            break;
        case BlockType::PLAYER:
            yes = dynamic_cast<Player*>(block) != nullptr;
            break;
        case BlockType::NPC:
            yes = dynamic_cast<NPC*>(block) != nullptr;
            break;
        case BlockType::WEAPON:
            yes = dynamic_cast<Weapon*>(block) != nullptr;
            break;
        case BlockType::BULLET:
            yes = dynamic_cast<Bullet*>(block) != nullptr;
            break;
        default:
            CCLOG("[Block::isThisBlock] unsupported block type: %d", type);
            break;
    }
    return yes;
}

bool Block::init() {
    if (!Sprite::init()) {
        return false;
    }
    _id = generateID();

    return true;
}

bool Block::containsPoint(const Vec2& point) const {
    return getBoundingBox().containsPoint(point);
}

bool Block::isDownFloorEmpty() const {
    int downFloor = getFloor() - 1;
    return !isBlockIntersection(downFloor);
}

bool Block::isMapBorderIntersection(Vector<Block*>* resultList) const {
    bool yes = false;
    auto pos = getPosition();
    int blockSize = getSize();
    if (resultList) {
        MapBorder* mapBorder = nullptr;
        if (pos.x - blockSize / 2.0f < 0) {
            mapBorder = MapBorder::LEFT();
        } else if (pos.y + blockSize / 2.0f > CENTER_HEIGHT) {
            mapBorder = MapBorder::TOP();
        } else if (pos.x + blockSize / 2.0f > CENTER_WIDTH) {
            mapBorder = MapBorder::RIGHT();
        } else if (pos.y - blockSize / 2.0f < 0) {
            mapBorder = MapBorder::BOTTOM();
        }
        if (mapBorder) {
            resultList->pushBack(mapBorder);
            yes = true;
        }
    } else {
        yes = pos.x - blockSize / 2.0f < 0
            || pos.y + blockSize / 2.0f > CENTER_HEIGHT
            || pos.x + blockSize / 2.0f > CENTER_WIDTH
            || pos.y - blockSize / 2.0f < 0;
    }
    return yes;
}

// TODO: resultList改为Set
bool Block::isBlockIntersection(int floor, Vector<Block*>* resultList) const {
    // 不同Floor的Block默认不产生碰撞
    bool yes = false;
    if (floor == NoneFloor) {
        floor = getFloor();
    }
    auto box = getBoundingBox();
    auto mapLayer = MapLayer::getInstance();
    auto otherBlocks = mapLayer->getAroundBlocks(this, floor);
    //printf("--------- otherBlocks size: %d\n", otherBlocks.size());
    for (auto iter : otherBlocks) {
        auto otherBlock = iter.second;
        //if (otherBlock->getType() == BlockType::PLAYER) {
        //    auto realPos = otherBlock->getPosition();
        //    printf(">>> otherBlock: mngmt pos: (%d, %d), real pos: (%d, %d)\n",
        //            (int)iter.first.x, (int)iter.first.y, (int)realPos.x, (int)realPos.y);
        //    printf(">>> this: 0x%x, otherBlock: 0x%x\n", this, otherBlock);
        //    printf(">>> this pos: (%d, %d)\n", (int)getPosition().x, (int)getPosition().y);
        //}
        if (otherBlock == this) {
            continue;
        }
        // 玩家及子弹和桥面及冰面不产生碰撞
        if ((getType() == BlockType::PLAYER || getType() == BlockType::BULLET) &&
                (otherBlock->getType() == BlockType::BRIDGE ||
                 otherBlock->getType() == BlockType::ICE)) {
            continue;
        }
        auto otherBox = otherBlock->getBoundingBox();
        if (box.equals(otherBox) || box.myIntersectsRect(otherBox)) {
            yes = true;
            if (resultList) {
                resultList->pushBack(otherBlock);
            } else {
                break;
            }
        }
    }
    return yes;
}

void Block::onBeCollided(Block* activeBlock) {
}

void Block::onCollidedWith(Vector<Block*>& withBlocks) {
}

void Block::onDestroyed() {
}

void Block::initSpriteFrameCache() {
    Campus::initSpriteFrameCache();
    Camp::initSpriteFrameCache();
    BlockWall::initSpriteFrameCache();
    BlockStone::initSpriteFrameCache();
    BlockForest::initSpriteFrameCache();
    BlockBridge::initSpriteFrameCache();
    BlockCloud::initSpriteFrameCache();
    BlockIce::initSpriteFrameCache();
    BlockTrench::initSpriteFrameCache();
    BlockRiver::initSpriteFrameCache();
}
