#pragma once

#include <set>
#include "cocos2d.h"
#include "Common.h"

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
    BARRIER,        // 河流、壕沟的边界
    CLOUD,          // 云朵，floor为99，可遮罩其它所有Block，子弹可穿过，不能被摧毁
    CAMP,           // 战斗营地，floor为0，玩家加入阵营后出生地，可被摧毁，被摧毁即失败
    CAMPUS,         // 玩家营地，floor为0，玩家出生地，不可被摧毁，可进入玩家自创剧本(故事线)
    IMMOVABLE_BLOCK,
    MOVABLE_BLOCK,
    GAMER,
    PLAYER,
    SPECTATOR,
    NPC,
    WEAPON,
    BULLET,
    ITEM,
    MAP_BORDER,     // MAP_BORDER, 地图边界
    ALL,            // 全部
    COUNT
};

const int NoneFloor = -999;
const bool IsActiveCollision = true;

class Gamer;

class Block : public cocos2d::Sprite {
public:
    static const int DefaultFloor = 0;

    static void initSpriteFrameCache();

    static int generateID();
    static int getDefaultFloor(BlockType bt);
    static Block* createBlock(BlockType type, Gamer* creator=nullptr);
    static bool isThisBlock(BlockType type, const Block* block);

public:
    bool init() override;             // 调用父类的init

    int id() const { return getTag(); }
    int getFloor() const { return getLocalZOrder(); }
    void setFloor(int floor) { setLocalZOrder(floor); }
    void increaseFloor() { setLocalZOrder(getLocalZOrder() + 1); }
    void decreaseFloor() { setLocalZOrder(getLocalZOrder() - 1); }
    void setCreator(Gamer* creator) { _creator = creator; }
    void unsetCreator() { _creator = nullptr; }
    Gamer* getCreator() const { return _creator; }

    virtual BlockType getType() const = 0;
    virtual bool movable() const = 0;
    virtual bool canBeDestroy() const { return true; }
    virtual int getSize() const { return BLOCK_SIZE * 2; }

    bool containsPoint(const cocos2d::Vec2& point) const;
    bool isDownFloorEmpty() const;
    bool isMapBorderIntersection(cocos2d::Vector<Block*>* resultList=nullptr) const;
    bool isBlockIntersection(int floor=NoneFloor, cocos2d::Vector<Block*>* resultList=nullptr) const;
    bool isPlayerIntersection(int floor=NoneFloor) const;

    // callbacks
    virtual void onBeCollided(Block* activeBlock);              // 被碰撞回调
    virtual void onCollidedWith(cocos2d::Vector<Block*>& withBlocks);    // (主动)碰撞到回调
    virtual void onDestroyed();

    virtual std::string getSpriteFrameName() const = 0;

protected:
    Gamer* _creator = nullptr;
};
