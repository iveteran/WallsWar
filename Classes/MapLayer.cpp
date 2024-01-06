#include "MapLayer.h"
#include "Player.h"
#include "Block.h"
#include "Bullet.h"
#include "GameScene.h"
#include "Camp.h"
#include "constants/CampConstants.h"

#include <string>

USING_NS_CC;

static MapLayer* _mapLayer = nullptr;

void MapLayer::initSpriteFrameCache() {
    // 方块
    Block::initSpriteFrameCache();

    // 子弹
    Bullet::initSpriteFrameCache();

    // 玩家
    Player::initSpriteFrameCache();
}

bool MapLayer::init() {
    if (!LayerColor::init()) {
        return false;
    }

    // 设置地图位置
    setContentSize(Size(CENTER_WIDTH, CENTER_HEIGHT));
    setIgnoreAnchorPointForPosition(false);
    setPosition(Director::getInstance()->getVisibleSize() / 2);

    // 设置背景颜色为黑色
    //this->initWithColor(Color4B(0, 0, 0, 255));

    return true;
}

MapLayer* MapLayer::getInstance() {
    if (!_mapLayer) {
        _mapLayer = MapLayer::create();
        _mapLayer->retain();
    }

    return _mapLayer;
}

bool MapLayer::addAndManageBlock(Block* block) {
    addChild(block);
    return manageBlock(block);
}

bool MapLayer::removeAndUnmanageBlock(Block* block) {
    removeChild(block);
    return unmanageBlock(block);
}

void MapLayer::addNode(Node* node) {
    addChild(node);
}

void MapLayer::removeNode(Node* node) {
    removeChild(node);
}

Vec3 MapLayer::_getBlockMngmtPosition(int64_t blockId) {
    auto iter = _blockIdPositionMap.find(blockId);
    if (iter != _blockIdPositionMap.end()) {
        return iter->second;
    }
    return Vec3::ZERO;
}

void MapLayer::_addBlockMngmtPosition(int64_t blockId, const Vec3& mngmtPos) {
    //_blockIdPositionMap.insert(std::make_pair(block->id(), mngmtPos));
    _blockIdPositionMap[blockId] = mngmtPos;  // update or add element
}

void MapLayer::_removeBlockMngmtPosition(int64_t blockId) {
    _blockIdPositionMap.erase(blockId);
}

bool MapLayer::manageBlock(Block* block) {
    bool success = _manageBlock(block, _floorPosBlocks);

    //if (block->getType() == BlockType::PLAYER) {
    //   success = _manageBlock(block, _floorPosPlayers);
    //} else if (Block::isThisBlock(BlockType::WEAPON, block)) {
    //    success = _manageBlock(block, _floorPosWeapons);
    //} else {
    //    //success = _manageBlock(block, _floorPosBlocks);
    //}
    return success;
}

bool MapLayer::unmanageBlock(Block* block) {
    BlockType type = block->getType();
    if (Block::isThisBlock(BlockType::WEAPON, block)) {
        type = BlockType::WEAPON;
    }

    // remove from blocks map
    bool success = false;
    auto mngmtPos = _getBlockMngmtPosition(block->id());
    if (mngmtPos != Vec3::ZERO) {
        success = _updateBlockPosition(mngmtPos, type, nullptr);
        _removeBlockMngmtPosition(block->id());
    }

    //if (block->getType() == BlockType::WALL) {
    //    printf("######## unmanageBlock, id: %d, type: %d, (%f, %f), success: %d\n", block->id(), block->getType(),
    //            block->getPosition().x, block->getPosition().y, success);
    //}
    return success;
}

bool MapLayer::_manageBlock(Block* block, FloorXYAxisBlockMap& floorPosBlocks) {
    int floor = block->getFloor();
    Vec2 blockPos = block->getPosition();
    int x = (int)blockPos.x;
    int y = (int)blockPos.y;
    auto iter = floorPosBlocks.find(floor);
    if (iter == floorPosBlocks.end()) {
        auto yAxisBlock = YAxisBlock{{y, block}};
        auto xyAxisBlock = XYAxisBlock{{x, yAxisBlock}};
        //floorPosBlocks.insert(std::make_pair(floor, xyAxisBlock));
        floorPosBlocks[floor] = xyAxisBlock;
    } else {
        auto xyAxisBlock = &(iter->second);
        auto iter2 = xyAxisBlock->find(x);
        if (iter2 == xyAxisBlock->end()) {
            auto yAxisBlock = YAxisBlock{{y, block}};
            //xyAxisBlock->insert(std::make_pair(x, yAxisBlock));
            (*xyAxisBlock)[x] = yAxisBlock;
        } else {
            auto yAxisBlock = &(iter2->second);
            //yAxisBlock->insert(std::make_pair(y, block));
            auto iter3 = yAxisBlock->find(y);
            if (iter3 == yAxisBlock->end()) {
                (*yAxisBlock)[y] = block;
            } else {
                // the position exists a block already
                return false;
            }
        }
    }

    Vec3 mngmtPos{(float)x, (float)y, (float)floor};
    _addBlockMngmtPosition(block->id(), mngmtPos);  // update or add element
    return true;
}

bool MapLayer::updateBlockPosition(MovableBlock* block) {
    BlockType type = block->getType();
    if (Block::isThisBlock(BlockType::WEAPON, block)) {
        type = BlockType::WEAPON;
    }
    auto mngmtPos = _getBlockMngmtPosition(block->id()); // get block current management position
    if (mngmtPos != Vec3::ZERO) {
        return _updateBlockPosition(mngmtPos, type, block);
    }
    return false;
}

bool MapLayer::_updateBlockPosition(const Vec3& mngmtPos, BlockType type, Block* block) {
    bool success = false;
    //switch (type) {
    //    case BlockType::PLAYER:
    //        success = _updateBlockPosition(mngmtPos, block, _floorPosPlayers);
    //        break;
    //    case BlockType::WEAPON:
    //        success = _updateBlockPosition(mngmtPos, block, _floorPosWeapons);
    //        break;
    //    default:
    //        //success = _updateBlockPosition(mngmtPos, block, _floorPosBlocks);
    //        break;
    //}
    success = _updateBlockPosition(mngmtPos, block, _floorPosBlocks);
    return success;
}

bool MapLayer::_updateBlockPosition(const Vec3& mngmtPos, Block* block,
        FloorXYAxisBlockMap& floorPosBlocks) {
    int floor = (int)mngmtPos.z;
    int x = (int)mngmtPos.x;
    int y = (int)mngmtPos.y;

    auto foundBlock = _removeBlockPosition(floor, x, y, floorPosBlocks);  // remove block current managemant position
    if (block && foundBlock && foundBlock == block) {  // 只有找到并删除原位置的才更新
    //if (block) {  // 没找到和删除原位置的就新增？
        //printf("######## position update (%d, %d, %d) -> (%d, %d, %d)\n", floor, x, y,
        //        block->getFloor(), (int)block->getPosition().x, (int)block->getPosition().y);
        //printf("######## updating OR adding? %s\n", foundBlock ? "updating" : "adding");
        //if (foundBlock->getType() == BlockType::WALL) {
        //    printf("######## foundBlock: id: %d, type: %d\n", foundBlock->id(), foundBlock->getType());
        //}
        return _manageBlock(block, floorPosBlocks);
    }
    return foundBlock != nullptr;
}

Block* MapLayer::_removeBlockPosition(int floor, int x, int y,
        FloorXYAxisBlockMap& floorPosBlocks) {
    Block* foundBlock = nullptr;
    auto iter = floorPosBlocks.find(floor);
    if (iter != floorPosBlocks.end()) {
        auto xyAxisPlayer = &(iter->second);
        auto iter2 = xyAxisPlayer->find(x);
        if (iter2 != xyAxisPlayer->end()) {
            auto yAxisPlayer = &(iter2->second);
            auto iter3 = yAxisPlayer->find(y);
            if (iter3 != yAxisPlayer->end()) {
                foundBlock = iter3->second;
                yAxisPlayer->erase(iter3);
                if (yAxisPlayer->empty()) {
                    xyAxisPlayer->erase(iter2);
                    if (xyAxisPlayer->empty()) {
                        floorPosBlocks.erase(iter);
                    }
                }
                //if (foundBlock->getType() == BlockType::WALL) {
                //    printf("######## removed id: %d, (%d, %d, %d)\n", foundBlock->id(), foundBlock->getFloor(),
                //            (int)foundBlock->getPosition().x, (int)foundBlock->getPosition().y);
                //}
            }
        }
    }
    return foundBlock;
}

CoordBlockMap
MapLayer::getInsideCircleBlocks(const Vec2& center, float radius,
        const BlockTypeSet& btSet, int floor) {
    auto origin = Vec2(center.x - radius, center.y - radius);
    float sideLength = radius * 2;
    auto posInCircleFilter = std::bind(isPointInCircle, center, radius, std::placeholders::_1);
    return getInsideSquareBlocks(origin, sideLength, btSet, posInCircleFilter, floor);
}

CoordBlockMap
MapLayer::getInsideSquareBlocks(const Vec2& origin, float sideLength,
        const BlockTypeSet& btSet, const BlockPositionFilter& positionFilter, int floor) {

    CoordBlockMap posBlocks;
    if (floor != NoneFloor) {
        auto iter = _floorPosBlocks.find(floor);
        if (iter != _floorPosBlocks.end()) {
            auto xyAxisBlock = iter->second;
            _getInsideSquareBlocks(posBlocks, origin, sideLength, btSet, positionFilter, xyAxisBlock);
        }
    } else {
        for (auto iter : _floorPosBlocks) {
            auto xyAxisBlock = iter.second;
            _getInsideSquareBlocks(posBlocks, origin, sideLength, btSet, positionFilter, xyAxisBlock);
        }
    }
    return posBlocks;
}

void MapLayer::_getInsideSquareBlocks(CoordBlockMap& posBlocks,/* output */
        const Vec2& origin, float sideLength, const BlockTypeSet& btSet,
        const BlockPositionFilter& positionFilter,
        const XYAxisBlock& xyAxisBlock) {

    auto iterBegin = xyAxisBlock.upper_bound(origin.x);
    auto iterEnd = xyAxisBlock.lower_bound(origin.x + sideLength);
    for (auto iter2 = iterBegin; iter2 != iterEnd; ++iter2) {
        float x = iter2->first;
        auto yAxisBlock = iter2->second;
        auto iter2Begin = yAxisBlock.upper_bound(origin.y);
        auto iter2End = yAxisBlock.upper_bound(origin.y + sideLength);
        for (auto iter3 = iter2Begin; iter3 != iter2End; ++iter3) {
            float y = iter3->first;
            auto gotBlock = iter3->second;
            if ((btSet.empty() || btSet.find(gotBlock->getType()) != btSet.end()) && // check contains
                    (!positionFilter || positionFilter(gotBlock->getPosition()))) {   // check in circle if the filter available
                posBlocks.insert(Vec2(x, y), gotBlock);
            }
        }
    }
}

CoordBlockMap
MapLayer::getAroundBlocks(const Block* block, int blockFloor, Direction dir) {
    CoordBlockMap posBlocks;

    if (blockFloor != NoneFloor) {
        auto iter = _floorPosBlocks.find(blockFloor);
        if (iter != _floorPosBlocks.end()) {
            auto xyAxisBlock = iter->second;
            _getAroundBlocks(posBlocks, block, xyAxisBlock, dir);
        }
    } else {
        for (auto iter : _floorPosBlocks) {
            auto xyAxisBlock = iter.second;
            _getAroundBlocks(posBlocks, block, xyAxisBlock, dir);
        }
    }
    return posBlocks;
}

void MapLayer::_getAroundBlocks(CoordBlockMap& posBlocks, const Block* block,
        const XYAxisBlock& xyAxisBlock, Direction dir) {
    Vec2 blockPos = block->getPosition();
    int xyOffset = block->getSize() / 2 + Block::MAX_SIZE;
    int length = block->getSize() + Block::MAX_SIZE * 2;
    int originX = blockPos.x - xyOffset;
    int originY = blockPos.y - xyOffset;

    auto iterBegin = xyAxisBlock.upper_bound(originX);
    auto iterEnd = xyAxisBlock.lower_bound(originX + length);
    for (auto iter2 = iterBegin; iter2 != iterEnd; ++iter2) {
        float x = iter2->first;
        auto yAxisBlock = iter2->second;
        auto iter2Begin = yAxisBlock.upper_bound(originY);
        auto iter2End = yAxisBlock.upper_bound(originY + length);
        for (auto iter3 = iter2Begin; iter3 != iter2End; ++iter3) {
            float y = iter3->first;
            auto gotBlock = iter3->second;
            //auto realPos = gotBlock->getPosition(); // (x, y) 可能和(gotBlock.x, gotBlock.y)不一样，
            //if (gotBlock->getType() == BlockType::PLAYER) {
            //    printf(">> _getAroundBlocks, mngmt pos: (%d, %d), real pos: (%d, %d)\n", (int)x, (int)y, (int)realPos.x, (int)realPos.y);
            //}
            if (gotBlock == block) {
                continue;  // 排除自己
            }
            posBlocks.insert(Vec2(x, y), gotBlock);
        }
    }
}

bool MapLayer::hasBlockAtPosition(const Vec2& pos, int floor) const {
    auto iter = _floorPosBlocks.find(floor);
    if (iter != _floorPosBlocks.end()) {
        auto xyAxisBlock = iter->second;
        auto iter2 = xyAxisBlock.find((int)pos.x);
        if (iter2 != xyAxisBlock.end()) {
            auto yAxisBlock = iter2->second;
            return yAxisBlock.find((int)pos.y) != yAxisBlock.end();
            //auto iter3 = yAxisBlock.find((int)pos.y);
            //if (iter3 != yAxisBlock.end()) {
            //    auto block = iter3->second;
            //    printf(">>> hasBlockAtPosition, type: %d, floor: %d, pos: (%f, %f)\n",
            //            block->getType(), floor, block->getPosition().x, block->getPosition().y);
            //    return true;
            //}
        }
    }
    return false;
}

void MapLayer::clearBlocks() {
    //_floorPosPlayers.clear();
    //_floorPosWeapons.clear();
    _floorPosBlocks.clear();
}

void MapLayer::resetMap() {
    clearBlocks();
    _mapData.clear();
    this->cleanup();
    this->removeAllChildrenWithCleanup(true);
}

void MapLayer::createCamps() {
    auto campPos = Vec2(CAMP_X, CAMP_Y); // 左下角
    _camp = createCamp(campPos);
    _player1 = _camp->addHost();

    auto enemyCampPos = Vec2(ENEMY_CAMP_X, ENEMY_CAMP_Y); // 右上角
    _enemyCamp = createCamp(enemyCampPos);
    _enemyCamp->enableAI();

    _camp->addEnemyCamp(_enemyCamp);
    _player1->addEnemyCamp(_enemyCamp);
}

Camp* MapLayer::createCamp(const Vec2& pos) {
    auto camp = Camp::create();
    camp->setPosition(pos);
    addAndManageBlock(camp);
    createCampusParapetWall(camp);
    return camp;
}

Camp* MapLayer::getCamp() const {
    return _camp;
}

Camp* MapLayer::getEnemyCamp() const {
    return _enemyCamp;
}

Player* MapLayer::getPlayer1() const {
    return _player1;
}

void MapLayer::createCampusParapetWall(const Camp* camp) {
    // 顺时针所有块，注：坐标原点在左下角, 如果在游戏地图外就不创建
    auto campPos = camp->getPosition();
    auto campManager = camp->getHost();
    // 上侧护墙，从左到右
    int x, y;
    y = campPos.y + Camp::SIZE / 2;
    if (y < CENTER_HEIGHT) {  // 如果在游戏地图外就不创建
        for (int x = (campPos.x - Camp::SIZE / 2); x < CENTER_WIDTH && x <= (campPos.x + Camp::SIZE / 2); x += Block::SIZE)
        {
            _addBlock((float)x, (float)y, BlockType::STONE, campManager);
        }
    }
    // 右侧护墙，从上到下
    x = campPos.x + Camp::SIZE / 2;
    if (x < CENTER_WIDTH) {
        for (int y = (campPos.y + Camp::SIZE / 2 - Block::SIZE); y >= 0 && y >= (campPos.y- Camp::SIZE / 2 - Block::SIZE); y -= Block::SIZE)
        {
            _addBlock((float)x, (float)y, BlockType::STONE, campManager);
        }
    }
    // 下侧护墙，从右到左
    y = campPos.y - Camp::SIZE / 2 - Block::SIZE;
    if (y >= 0) {  // 如果在游戏地图外就不创建
        for (int x = (campPos.x + Camp::SIZE / 2 - Block::SIZE); x >= 0 && x >= (campPos.x - Camp::SIZE / 2 - Block::SIZE); x -= Block::SIZE)
        {
            _addBlock((float)x, (float)y, BlockType::STONE, campManager);
        }
    }
    // 左侧护墙，从下到上
    x = campPos.x - Camp::SIZE / 2 - Block::SIZE;
    if (x >= 0) {
        for (int y = (campPos.y - Camp::SIZE / 2); y < CENTER_HEIGHT && y <= (campPos.y + Camp::SIZE / 2); y += Block::SIZE)
        {
            _addBlock((float)x, (float)y, BlockType::STONE, campManager);
        }
    }
}

bool MapLayer::loadMapData() {
    // TODO: load from store
    int widthSize = (int)(CENTER_WIDTH / Block::SIZE);
    int heightSize = (int)(CENTER_HEIGHT / Block::SIZE);
    // 增加底边围墙
    int offset = 4;
    int y = offset;
    for (int x=offset; x<widthSize-1-offset; x++)
    {
        _addBlock(x, y, BlockType::WALL);
    }
    // 增加右边围墙
    int x = widthSize-1-offset;
    for (int y=offset; y<heightSize-1-offset; y++)
    {
        _addBlock(x, y, BlockType::WALL);
    }
    // 增加顶边围墙
    y = heightSize-1-offset;
    for (int x=offset; x<widthSize-offset; x++)
    {
        _addBlock(x, y, BlockType::WALL);
    }
    // 增加左边围墙
    x = offset;
    for (int y=offset; y<heightSize-offset; y++)
    {
        _addBlock(x, y, BlockType::WALL);
    }

    // 中心点石头
    _addBlock(widthSize / 2, heightSize / 2, BlockType::STONE);

    return true;
}

bool MapLayer::_addBlock(float x, float y, BlockType t, Actor* actor) {
    Vec2 blockPos(x, y);
    int floor = Block::getDefaultFloor(t);
    if (hasBlockAtPosition(blockPos, floor)) {
        CCLOG("[MapLayer::_addBlock] already has block at: (%f, %f) and floor: %d, do not to create again!",
                blockPos.x, blockPos.y, floor);
        return false;
    }

    // 创建不同类型的方块
    Block* block = Block::createBlock(t);
    if (block) {
        block->setCreator(actor);
        block->setPosition(blockPos);
        addAndManageBlock(block);

        return true;
    }
    return false;
}

bool MapLayer::_addBlock(int i, int j, BlockType t, Actor* actor) {
    return _addBlock((float)i * Block::SIZE, (float)j * Block::SIZE, t, actor);
}

bool MapLayer::createBlock(int i, int j, BlockType t, Actor* actor) {
    return _addBlock(i, j, t, actor);
}

bool MapLayer::createBlock(float x, float y, BlockType t, Actor* actor) {
    return _addBlock(x, y, t, actor);
}

bool MapLayer::createBlock(const Vec2& pos, BlockType t, Actor* actor) {
    return createBlock(pos.x, pos.y, t, actor);
}

int MapLayer::createBlocks(const std::vector<Vec2>& posList, BlockType t, Actor* actor) {
    int count = 0;
    for (auto pos : posList) {
        bool success = createBlock(pos.x, pos.y, t, actor);
        if (success) {
            count++;
        }
    }
    return count;
}

void MapLayer::loadLevelData(short stage) {
    // 清理工作
    resetMap();

    // 添加大本营
    createCamps();

    // 加载地图
    loadMapData();
}

const std::string&
MapLayer::getMapData() {
    return _mapData;
}
