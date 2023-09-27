#pragma once

#include "Common.h"
#include "cocos2d.h"
#include <utility>

// 方块分类
enum class BlockCategory {
    OBSTACLE,                     // 坦克不能通过，子弹可以摧毁
    NON_OBSTACLE,                 // 坦克可以通过，子弹不能摧毁
    RIVER                         // 坦克不能通过，子弹不能摧毁
};

// 方块类型
enum class BlockType {
    UNDEFINED,
    WALL,           // 砖头，floor为0，玩家不能通过，子弹可摧毁(一层)，强力子弹可摧毁(两层)
    STONE,          // 石头，floor为0，玩家不能通过，子弹不能摧毁，强力子弹可摧毁(一层)
    FOREST,         // 树木，floor为9，会撤档floor比它小的Block，子弹可穿过，可被燃烧弹摧毁，玩家可砍伐并生成木头(Wood)且可携带
    BRIDGE,         // 木桥，floor为0，由木头(Wood)生成，只能在河流上创建，子弹可穿过，可被燃烧弹摧毁
    ICE,            // 冰面，floor为0，玩家在上面行走会加速，子弹可穿过，可被燃烧弹摧毁
    RIVER,          // 河流，floor为-1，玩家会掉落，3秒内未上岸将被淹死，可以用Wood搭桥，子弹可穿过
    TRENCH,         // 壕沟，floor为-1，玩家会掉落，玩家可挖掘并生成土方(Soil)且可携带，只能使用土方填埋才可以上来，子弹可穿过
    CLOUD,          // 云朵，floor为99，可遮罩其它所有Block，子弹可穿过，不能被摧毁
    CAMP,           // 战斗营地，floor为0，玩家加入阵营后出生地，可被摧毁，被摧毁即失败
    CAMPUS,         // 玩家营地，floor为0，玩家出生地，不可被摧毁，可进入玩家自创剧本(故事线)
    COUNT
};


class Block : public cocos2d::Sprite {
public:
    bool init() override;             // 调用父类的init

    virtual BlockCategory getCategory() = 0;
    virtual BlockType getType() { return BlockType::UNDEFINED; }
    int getFloor() const { return getLocalZOrder(); }
    void setFloor(int floor) { setLocalZOrder(floor); }

    static int getFloor(BlockType bt);
    static Block* createBlock(BlockType type);
    static void addSpriteFrameCache();

    static const int Floor = 0;
};

class __Obstacle : public Block {
public:
    BlockCategory getCategory() override { return BlockCategory::OBSTACLE; }
};

class __NonObstacle : public Block {
public:
    BlockCategory getCategory() override { return BlockCategory::NON_OBSTACLE; }
};

class __River : public Block {
public:
    BlockCategory getCategory() override { return BlockCategory::RIVER; }
};

class BlockCamp : public __Obstacle {
public:
    bool init() override;
    BlockType getType() override { return BlockType::CAMP; }

    CREATE_FUNC(BlockCamp);
};

class BlockWall : public __Obstacle {
public:
    bool init() override;
    BlockType getType() override { return BlockType::WALL; }

    std::pair<bool, bool> destory(Direction dir, const cocos2d::Rect& box);

    CREATE_FUNC(BlockWall);

    static const int Floor = 0;

private:
    cocos2d::LayerColor* blacks[4]{};        // 4个黑色方块，用于遮挡
    bool _isDestory();                      // 检测方块是否被摧毁
};

class BlockStone : public __Obstacle {
public:
    bool init() override;
    BlockType getType() override { return BlockType::STONE; }

    CREATE_FUNC(BlockStone);

    static const int Floor = 0;
};

class BlockForest : public __NonObstacle {
public:
    bool init() override;
    BlockType getType() override { return BlockType::FOREST; }

    CREATE_FUNC(BlockForest);

    static const int Floor = 9;
};

class BlockBridge : public __NonObstacle {
public:
    bool init() override;
    BlockType getType() override { return BlockType::BRIDGE; }

    CREATE_FUNC(BlockBridge);

    static const int Floor = 0;
};

class BlockCloud : public __NonObstacle {
public:
    bool init() override;
    BlockType getType() override { return BlockType::CLOUD; }

    CREATE_FUNC(BlockCloud);

    static const int Floor = 99;
};

class BlockIce : public __NonObstacle {
public:
    bool init() override;
    BlockType getType() override { return BlockType::ICE; }

    CREATE_FUNC(BlockIce);

    static const int Floor = 0;
};

class BlockTrench : public __NonObstacle {
public:
    bool init() override;
    BlockType getType() override { return BlockType::TRENCH; }

    CREATE_FUNC(BlockTrench);

    static const int Floor = -1;
};

class BlockRiver : public __River {
public:
    bool init() override;
    BlockType getType() override { return BlockType::RIVER; }

    CREATE_FUNC(BlockRiver);

    static const int Floor = -1;
};
