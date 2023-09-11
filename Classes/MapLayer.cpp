#include "Common.h"

#include "MapLayer.h"
#include "PlayerTank.h"
#include "EnemyTank.h"
#include "Block.h"
#include "RandomUtil.h"
#include "GameScene.h"

#include <string>

USING_NS_CC;

static MapLayer* _mapLayer = nullptr;

bool MapLayer::init() {
    if (!cocos2d::LayerColor::init()) {
        return false;
    }

    // 设置背景颜色为黑色
    this->initWithColor(Color4B(0, 0, 0, 255));

    // 加载精灵帧缓存
    _addSpriteFrameCache();

    return true;
}

MapLayer* MapLayer::getInstance() {
    if (!_mapLayer) {
        _mapLayer = MapLayer::create();
        _mapLayer->retain();
    }

    return _mapLayer;
}

void MapLayer::_addSpriteFrameCache() {
    // 右侧信息区域
    GameScene::addSpriteFrameCache();

    // 方块
    Block::addSpriteFrameCache();

    // 子弹
    Bullet::addSpriteFrameCache();

    // 坦克
    TankBase::addSpriteFrameCache();
    PlayerTank::loadFrameAnimation();
    EnemyTank::loadFrameAnimation();
}

void MapLayer::_addEnemy(float x, float y) {
    for (const auto& player : _players) {
        if (player->getBoundingBox().containsPoint({ x, y }))
            return;
    }

    for (const auto& enemy : _enemies) {
        if (enemy->getBoundingBox().containsPoint({ x, y }))
            return;
    }

    auto enemyTank = EnemyTank::create();
    this->addChild(enemyTank);
    enemyTank->setPosition(x, y);

    auto bullets = enemyTank->getAllBullets();
    for (auto bullet : bullets) {
        this->addChild(bullet);
    }

    _enemies.pushBack(enemyTank);
    remainTank--;

    auto parent = dynamic_cast<GameScene*>(this->getParent());
    if (parent) {
        parent->updateInformationArea();
    }
}

void MapLayer::autoAddEnemies(float) {
    addEnemies();
}

void MapLayer::autoControlEnemiesDirection(float) {
    for (auto enemy : _enemies) {
        enemy->changeDirection();
    }
}

void MapLayer::autoControlEnemiesShoot(float) {
    for (auto enemy : _enemies) {
        // 三分之一的概率发射子弹
        if (RandomUtil::random(1, 3) == 1)
            enemy->shoot();
    }
}

PlayerTank* MapLayer::addPlayer() {
    auto player = PlayerTank::create();
    this->addChild(player);
    player->setPosition(PLAYER1_START_X, PLAYER1_START_Y);

    auto bullets = player->getAllBullets();
    for (auto& bullet : bullets) {
        this->addChild(bullet, 100);
    }

    _players.pushBack(player);
    return player;
}

void MapLayer::addEnemies() {
    // 初始时添加3辆坦克
    if (remainTank == ENEMIES_COUNT) {
        _addEnemy(ENEMY1_STAR_X, ENEMY1_STAR_Y);
        _addEnemy(ENEMY2_STAR_X, ENEMY2_STAR_Y);
        _addEnemy(ENEMY3_STAR_X, ENEMY3_STAR_Y);
    } else {
        if (remainTank == 0) return;

        // 当坦克数量小于6辆时
        if (_enemies.size() < 6) {
            // 随机添加一辆
            switch (RandomUtil::random(0, 2)) {
            case 0:
                _addEnemy(ENEMY1_STAR_X, ENEMY1_STAR_Y);
                break;
            case 1:
                _addEnemy(ENEMY2_STAR_X, ENEMY2_STAR_Y);
                break;
            case 2:
                _addEnemy(ENEMY3_STAR_X, ENEMY3_STAR_Y);
                break;
            default:
                break;
            }
        }
    }
}

void MapLayer::resetMap() {
    _blocks.clear();
    _data.clear();
    _enemies.clear();
    _players.clear();
    this->cleanup();
    this->removeAllChildrenWithCleanup(true);
    this->remainTank = ENEMIES_COUNT;
    this->isCampOk = true;
}

void MapLayer::loadLevelData(short stage) {
    // 清理工作
    resetMap();

    // 先添加大本营
    auto camp = BlockCamp::create();
    this->addChild(camp);
    camp->setPosition(CAMP_X, CAMP_Y);
    _blocks.pushBack(camp);

    // 然后添加其他方块
    std::string filename = "maps/" + std::to_string(stage) + ".txt";
    _data = FileUtils::getInstance()->getStringFromFile(filename);

    int index = 0;

    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            char c = _data[index++];
            if (c == '\r') {
                j--;
                index++;
                continue;
            }
            // 制造精灵
            Block* block = nullptr;

            // 创建不同类型的方块
            if (c == '3') {
                block = BlockWall::create();
            } else if (c == '5') {
                block = BlockStone::create();
            } else if (c == '1') {
                block = BlockForest::create();
            } else if (c == '4') {
                block = BlockRiver::create();
            } else if (c == '2') {
                block = BlockIce::create();
            }

            if (block) {
                // 将精灵添加到图层
                if (block->getType() == BlockType::FOREST) {
                    this->addChild(block, 101);
                } else {
                    this->addChild(block);
                }

                // 设置精灵在图层上的位置
                block->setAnchorPoint(Vec2(0, 0));
                block->setPosition(Vec2((float)j * BLOCK_SIZE, (float)(25 - i) * BLOCK_SIZE));

                // 存储vector
                _blocks.pushBack(block);
            }

        }
    }
}

PlayerTank* MapLayer::getPlayer1() {
    if (_players.size() > 0)
        return _players.at(0);
    return nullptr;
}

Block* MapLayer::getCamp() {
    return _blocks.at(0);
}

cocos2d::Vector<Block*>& MapLayer::getAllBlocks() {
    return _blocks;
}

cocos2d::Vector<EnemyTank*>& MapLayer::getEnemies() {
    return _enemies;
}

cocos2d::Vector<PlayerTank*>& MapLayer::getPlayers() {
    return _players;
}

void MapLayer::enableAutoAddEnemies(bool b) {
    if (b) {
        // 每隔4.5秒添加一名敌人
        this->schedule(CC_SCHEDULE_SELECTOR(MapLayer::autoAddEnemies), 4.5f);
    } else {
        this->unschedule(CC_SCHEDULE_SELECTOR(MapLayer::autoAddEnemies));
    }
}

void MapLayer::enableAutoControlEnemies(bool b) {
    if (b) {
        this->schedule(CC_SCHEDULE_SELECTOR(MapLayer::autoControlEnemiesDirection), 0.1f);
        this->schedule(CC_SCHEDULE_SELECTOR(MapLayer::autoControlEnemiesShoot), 0.5f);
    } else {
        this->unschedule(CC_SCHEDULE_SELECTOR(MapLayer::autoControlEnemiesDirection));
        this->unschedule(CC_SCHEDULE_SELECTOR(MapLayer::autoControlEnemiesShoot));
    }
}

const std::string& MapLayer::getMapData() {
    return _data;
}
