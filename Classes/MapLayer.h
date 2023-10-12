#pragma once

#include "cocos2d.h"
#include "Block.h"
#include "Vec2Hash.h"

#include <map>
#include <string>

using cocos2d::Rect;
using cocos2d::Vector;
using cocos2d::Map;
using cocos2d::Vec2;
using cocos2d::Vec3;

using YAxisBlock = std::map<int, Block*>;
using XYAxisBlock = std::map<int, YAxisBlock>;
using FloorXYAxisBlockMap = std::map<int, XYAxisBlock>;
using CoordBlockMap = Map<Vec2, Block*>;

class MovableBlock;
class Camp;
class Player;

class MapLayer : public cocos2d::LayerColor {
public:
    bool init() override;

    static MapLayer* getInstance();                 // 得到地图图层实例

    //void loadCamp();
    bool loadMapData();
    void loadLevelData(short stage);                // 加载指定关卡的数据

    void createCamps();
    Camp* createCamp(const cocos2d::Vec2& pos);
    Camp* getCamp() const;
    Camp* getEnemyCamp() const;
    void createCampusParapetWall(const Camp* camp);

    Player* getPlayer1() const;                       // 得到玩家1
    const std::string& getMapData();                // 得到地图数据
    CoordBlockMap getAroundBlocks(const Block* block, int blockFloor,
            Direction dir=Direction::NONE);

    void addAndManageBlock(Block* block);
    void removeAndUnmanageBlock(Block* block);

    Vec3 getBlockMngmtPosition(int64_t blockId);
    void manageBlock(Block* block);
    void unmanageBlock(Block* block);
    bool updateBlockPosition(MovableBlock* block);
    bool hasBlockAtPosition(const cocos2d::Vec2& pos, int floor=0) const;

    bool createBlock(int i, int j, BlockType t, Gamer* gamer=nullptr);
    bool createBlock(float x, float y, BlockType t, Gamer* gamer=nullptr);
    bool createBlock(const cocos2d::Vec2& pos, BlockType t, Gamer* gamer=nullptr);
    int createBlocks(const std::vector<cocos2d::Vec2>& posList, BlockType t, Gamer* gamer=nullptr);

    void resetMap();                                // 清理工作
    void clearBlocks();

private:
    CREATE_FUNC(MapLayer);                          // 单例对象

    bool _addBlock(float x, float y, BlockType t, Gamer* gamer=nullptr);
    bool _addBlock(int i, int j, BlockType t, Gamer* gamer=nullptr);

    void _manageBlock(Block* block, FloorXYAxisBlockMap& floorPosBlocks);
    bool _updateBlockPosition(const Vec3& updatePos, BlockType type, Block* block);
    bool _updateBlockPosition(const Vec3& updatePos, Block* block, FloorXYAxisBlockMap& floorPosBlocks);
    Block* _removeBlockPosition(int floor, int x, int y,  FloorXYAxisBlockMap& floorPosBlocks);

    void _getAroundBlocks(CoordBlockMap& posBlocks, const Block* block,
            const XYAxisBlock& xyAxisBlock, Direction dir=Direction::NONE);

    void initSpriteFrameCache();                   // 加载精灵帧缓存

private:
    FloorXYAxisBlockMap _floorPosBlocks;
    FloorXYAxisBlockMap _floorPosPlayers;
    FloorXYAxisBlockMap _floorPosWeapons;
    std::unordered_map<int64_t, Vec3> _blockIdPositionMap;

    std::string _mapData;                               // 地图数据

    Camp* _camp = nullptr;
    Camp* _enemyCamp = nullptr;
    Player* _player1 = nullptr;
};
