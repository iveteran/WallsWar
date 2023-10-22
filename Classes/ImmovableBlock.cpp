#include "ImmovableBlock.h"
#include "Bullet.h"
#include "MapLayer.h"

USING_NS_CC;

static MapBorder* _mbTop;
static MapBorder* _mbRight;
static MapBorder* _mbBottom;
static MapBorder* _mbLeft;

MapBorder* MapBorder::TOP() {
    _mbTop = MapBorder::create();
    _mbTop->direction = Direction::UP;
    return _mbTop;
}

MapBorder* MapBorder::RIGHT() {
    _mbRight = MapBorder::create();
    _mbRight->direction = Direction::RIGHT;
    return _mbRight;
}

MapBorder* MapBorder::BOTTOM() {
    _mbBottom = MapBorder::create();
    _mbBottom->direction = Direction::DOWN;
    return _mbBottom;
}

MapBorder* MapBorder::LEFT() {
    _mbLeft = MapBorder::create();
    _mbLeft->direction = Direction::LEFT;
    return _mbLeft;
}

bool MapBorder::init() {
    if (!ImmovableBlock::init()) {
        return false;
    }

    setFloor(MapBorder::DefaultFloor);

    return true;
}

void BlockWall::initSpriteFrameCache() {
    auto wall = Sprite::create("images/block/wall.png")->getSpriteFrame();
    wall->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(wall, "wall");
}

// 一个墙面由4个砖块组成
bool BlockWall::init() {
    if (!ImmovableBlock::init()) {
        return false;
    }

    setFloor(BlockWall::DefaultFloor);
    initWithSpriteFrameName("wall");

    // 创建4个遮挡精灵，分别遮挡4个砖块
    for (int i = 0; i < 4; i++) {
        auto block = LayerColor::create(Color4B(0, 0, 0, 255), BRICK_SIZE, BRICK_SIZE);
        block->setIgnoreAnchorPointForPosition(false);
        block->setAnchorPoint(Vec2(0, 0));
        addChild(block);
        block->setVisible(false);

        block->setPosition(Vec2(i / 2 * BRICK_SIZE, i % 2 * BRICK_SIZE));
        _blacks[i] = block;
    }

    return true;
}

std::pair<int, bool>
BlockWall::destory(Direction dir, const Rect& box) {
    int numCollided = 0; // 子弹发生碰撞的砖块数
    auto position = getPosition();

    int moveAnElem = 0;  // 用于子弹向前移动一个已经摧毁(已隐藏)的砖块
    for (int i = 0; i < 4; i++) {

        // 转为坐标为相对于MapLayer的坐标
        auto preBox = _blacks[i]->getBoundingBox();
        auto tranBox = Rect(preBox.getMinX() + position.x,
                            preBox.getMinY() + position.y,
                            preBox.getMaxX() - preBox.getMinX(),
                            preBox.getMaxY() - preBox.getMinY());

        // 加宽子弹
        Rect cmpBox;
        switch (dir) {
        case Direction::LEFT:
            if (_blacks[2]->isVisible() && _blacks[3]->isVisible()) {
                moveAnElem = BRICK_SIZE * -1;
            }
        case Direction::RIGHT:
            if (moveAnElem == 0) {
                if (_blacks[0]->isVisible() && _blacks[1]->isVisible()) {
                    moveAnElem = BRICK_SIZE;
                }
            }
            cmpBox = Rect(box.getMinX() + moveAnElem, box.getMinY() + 1 - BLOCK_SIZE, BULLET_SIZE, TANK_SIZE);
            break;
        case Direction::UP:
            if (_blacks[0]->isVisible() && _blacks[2]->isVisible()) {
                moveAnElem = BRICK_SIZE;
            }
        case Direction::DOWN:
            if (moveAnElem == 0) {
                if (_blacks[1]->isVisible() && _blacks[3]->isVisible()) {
                    moveAnElem = BRICK_SIZE * -1;
                }
            }
            cmpBox = Rect(box.getMinX() + 1 - BLOCK_SIZE, box.getMinY() + moveAnElem, TANK_SIZE, BULLET_SIZE);
            break;
        default:
            break;
        }

        if (!_blacks[i]->isVisible() && tranBox.intersectsRect(cmpBox)) {
            _blacks[i]->setVisible(true);
            ++numCollided;
            if (numCollided == 2) {
                // 最多一次摧毁2个砖块，因为子弹是侧面到达
                break;
            }
        }
    }

    return std::make_pair(numCollided, _isDestory());
}

bool BlockWall::_isDestory() {
    for (int i = 0; i < 4; i++) {
        if (!_blacks[i]->isVisible()) {
            return false;
        }
    }

    return true;
}

void BlockWall::onBeCollided(Block* activeBlock) {
    if (!activeBlock) return;

    auto bullet = dynamic_cast<Bullet*>(activeBlock);
    if (bullet) {
        auto bulletDir = bullet->getDirection();
        auto bulletBox = bullet->getBoundingBox();
        auto result = destory(bulletDir, bulletBox);
        //printf(">> [BlockWall] destroy result: (%d, %d)\n", result.first, result.second);
        if (result.first > 0) {
            // 发生碰撞
            if (result.second) {
                // 发生碰撞且被摧毁
                MapLayer::getInstance()->removeAndUnmanageBlock(this);
                //printf(">> [BlockWall] removed, id: %d\n", id());
            }
        }
    }
}

void BlockStone::initSpriteFrameCache() {
    auto stone = Sprite::create("images/block/stone.png")->getSpriteFrame();
    stone->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(stone, "stone");
}

bool BlockStone::init() {
    if (!ImmovableBlock::init()) {
        return false;
    }

    setFloor(BlockStone::DefaultFloor);
    initWithSpriteFrameName("stone");

    return true;
}

void BlockStone::onBeCollided(Block* activeBlock) {
    if (!activeBlock) return;

    auto bullet = dynamic_cast<Bullet*>(activeBlock);
    if (bullet && bullet->getLevel() > 1) {
        MapLayer::getInstance()->removeAndUnmanageBlock(this);
    }
}

void BlockForest::initSpriteFrameCache() {
    auto forest = Sprite::create("images/block/forest.png")->getSpriteFrame();
    forest->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(forest, "forest");
}

bool BlockForest::init() {
    if (!ImmovableBlock::init()) {
        return false;
    }

    setFloor(BlockForest::DefaultFloor);
    initWithSpriteFrameName("forest");

    return true;
}

void BlockBridge::initSpriteFrameCache() {
    auto bridge = Sprite::create("images/block/bridge.png")->getSpriteFrame();
    bridge->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(bridge, "bridge");
}

bool BlockBridge::init() {
    if (!ImmovableBlock::init()) {
        return false;
    }

    setFloor(BlockBridge::DefaultFloor);
    initWithSpriteFrameName("bridge");

    return true;
}

void BlockCloud::initSpriteFrameCache() {
    auto cloud = Sprite::create("images/block/cloud.png")->getSpriteFrame();
    cloud->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(cloud, "cloud");
}

bool BlockCloud::init() {
    if (!ImmovableBlock::init()) {
        return false;
    }

    setFloor(BlockCloud::DefaultFloor);
    initWithSpriteFrameName("cloud");

    return true;
}

void BlockIce::initSpriteFrameCache() {
    auto ice = Sprite::create("images/block/ice.png")->getSpriteFrame();
    ice->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(ice, "ice");
}

bool BlockIce::init() {
    if (!ImmovableBlock::init()) {
        return false;
    }

    setFloor(BlockIce::DefaultFloor);
    initWithSpriteFrameName("ice");

    return true;
}

void BlockTrench::initSpriteFrameCache() {
    auto trench = Sprite::create("images/block/trench.png")->getSpriteFrame();
    trench->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(trench, "trench");
}

bool BlockTrench::init() {
    if (!ImmovableBlock::init()) {
        return false;
    }

    setFloor(BlockTrench::DefaultFloor);
    initWithSpriteFrameName("trench");

    return true;
}

void BlockRiver::initSpriteFrameCache() {
    auto spriteFrameCache = SpriteFrameCache::getInstance();
    auto river_0 = Sprite::create("images/block/river-0.png")->getSpriteFrame();
    auto river_1 = Sprite::create("images/block/river-1.png")->getSpriteFrame();
    river_0->getTexture()->setAliasTexParameters();
    river_1->getTexture()->setAliasTexParameters();
    spriteFrameCache->addSpriteFrame(river_0, "river_0");
    spriteFrameCache->addSpriteFrame(river_1, "river_1");
}

bool BlockRiver::init() {
    if (!ImmovableBlock::init()) {
        return false;
    }
    setFloor(BlockRiver::DefaultFloor);

    auto spriteFrameCache = SpriteFrameCache::getInstance();
    auto animation = Animation::createWithSpriteFrames({
        spriteFrameCache->getSpriteFrameByName("river_0"),
        spriteFrameCache->getSpriteFrameByName("river_1")
        },
        1.5f
    );
    auto animate = Animate::create(animation);
    runAction(RepeatForever::create(animate));

    return true;
}
