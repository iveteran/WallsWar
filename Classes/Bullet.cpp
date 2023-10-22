#include "AudioEngine.h"

#include "Common.h"
#include "Bullet.h"
#include "Player.h"
#include "MapLayer.h"

USING_NS_CC;

std::set<BlockType>
Bullet::CollidingAbleBlockTypes{
    BlockType::MAP_BORDER,
    BlockType::PLAYER,
    BlockType::BULLET,
    BlockType::WALL,
    BlockType::STONE,
    BlockType::CAMP
};

std::set<BlockType>
Bullet::getCollidingAbleBTs() const {
    return Bullet::CollidingAbleBlockTypes;
}

void Bullet::initSpriteFrameCache() {
    auto spriteFrameCache = SpriteFrameCache::getInstance();

    // 子弹
    auto bullet_l = Sprite::create("images/bullet/bullet-0.png")->getSpriteFrame();
    auto bullet_u = Sprite::create("images/bullet/bullet-1.png")->getSpriteFrame();
    auto bullet_r = Sprite::create("images/bullet/bullet-2.png")->getSpriteFrame();
    auto bullet_d = Sprite::create("images/bullet/bullet-3.png")->getSpriteFrame();

    bullet_l->getTexture()->setAliasTexParameters();
    bullet_u->getTexture()->setAliasTexParameters();
    bullet_r->getTexture()->setAliasTexParameters();
    bullet_d->getTexture()->setAliasTexParameters();

    spriteFrameCache->addSpriteFrame(bullet_l, "bullet_l");
    spriteFrameCache->addSpriteFrame(bullet_u, "bullet_u");
    spriteFrameCache->addSpriteFrame(bullet_r, "bullet_r");
    spriteFrameCache->addSpriteFrame(bullet_d, "bullet_d");

    // 子弹爆炸
    auto bumb_0 = Sprite::create("images/bullet/bumb0.png")->getSpriteFrame();
    auto bumb_1 = Sprite::create("images/bullet/bumb1.png")->getSpriteFrame();
    auto bumb_2 = Sprite::create("images/bullet/bumb2.png")->getSpriteFrame();

    bumb_0->getTexture()->setAliasTexParameters();
    bumb_1->getTexture()->setAliasTexParameters();
    bumb_2->getTexture()->setAliasTexParameters();

    spriteFrameCache->addSpriteFrame(bumb_0, "bumb_0");
    spriteFrameCache->addSpriteFrame(bumb_1, "bumb_1");
    spriteFrameCache->addSpriteFrame(bumb_2, "bumb_2");
}

bool Bullet::init() {
    if (!Weapon::init()) {
        return false;
    }

    setFiring(false);
    //setPosition(1.0f, 1.0f); // NOTE: 将初始地址设为(1, 1)，避免在MapLayer更新位置时被忽略(值为Vec2(0, 0))
    MapLayer::getInstance()->addNode(this);

    return true;
}

void Bullet::destroy() {
    onDestroyed();
    MapLayer::getInstance()->removeNode(this);
}

void Bullet::_recycle() {
    setFiring(false);
    stopMove();
    MapLayer::getInstance()->unmanageBlock(this);

    // 如果子弹的创建者(如玩家)不存在了就真实销毁它
    if (!_creator) {
        destroy();
    }
}

void Bullet::setLevel(int level) {
    _level = level;
}

void Bullet::setBeIntersection(bool value) {
    _isBeIntersection = value;
}

bool Bullet::getBeIntersection() const {
    return _isBeIntersection;
}

void Bullet::shoot(const Vec2& startPos, Direction dir, int floor) {
    switch (dir) {
    case Direction::LEFT:
        setSpriteFrame("bullet_l");
        break;
    case Direction::UP:
        setSpriteFrame("bullet_u");
        break;
    case Direction::RIGHT:
        setSpriteFrame("bullet_r");
        break;
    case Direction::DOWN:
        setSpriteFrame("bullet_d");
        break;
    default:
        break;
    }

    //printf(">> Player at (%d, %d) shoot bullet at (%d, %d)\n",
    //        (int)_creator->getPosition().x, (int)_creator->getPosition().y,
    //        (int)startPos.x, (int)startPos.y);
    setPosition(startPos);
    setFloor(floor);
    bool success = MapLayer::getInstance()->manageBlock(this);
    if (success) {
        setFiring(true);
        startMove(dir);
    } else {
        CCLOG("[Bullet::shoot] 子弹发射失败，离目标太近");
    }
}

void Bullet::_showEffect() {
    auto spriteFrameCache = SpriteFrameCache::getInstance();
    auto mapLayer = MapLayer::getInstance();

    auto animation = Animation::createWithSpriteFrames({
        spriteFrameCache->getSpriteFrameByName("bumb_0"),
        spriteFrameCache->getSpriteFrameByName("bumb_1"),
        spriteFrameCache->getSpriteFrameByName("bumb_2")
                                                       },
                                                       0.05f);

    auto effect = Sprite::create();
    mapLayer->addNode(effect);
    effect->setPosition(getPosition());

    effect->runAction(Sequence::create(
        Animate::create(animation),
        CallFunc::create([=]() {
            mapLayer->removeNode(effect);
        }),
        nullptr)
    );
}

void Bullet::_playEffectVoice() {
    // play effect voice
    auto creator = dynamic_cast<Player*>(_creator);
    if (creator && creator->isHost()) {
        auto voicePath = getVoicePath();
        if (voicePath) {
            AudioEngine::play2d(voicePath);
        }
    }
}

int Bullet::getMovingStep() const {
    int step = _creator ? _creator->getMovingStep() + _level : 1;
    return step;
}

void Bullet::onBeCollided(Block* activeBlock) {
    if (activeBlock->getType() == BlockType::BULLET) {
        //printf(">>> [Bullet::onBeCollided] block id: %d\n", activeBlock->id());
    }
    // 忽略玩家对自己子弹的碰撞
    if (activeBlock->getType() == BlockType::PLAYER && _creator != nullptr &&
            dynamic_cast<Player*>(activeBlock)->id() == _creator->id()) {
        return;
    }
    _recycle();
}

void Bullet::onCollidedWith(Vector<Block*>& withBlocks) {
    bool showEffect = false;
    int numIgnoreBlocks = 0;
    for (auto block : withBlocks) {
        if (block->getType() == BlockType::PLAYER) {
            auto player = dynamic_cast<Player*>(block);
            if (_creator && (_creator == player || // 击中的是自己
                    _creator->getCamp() == player->getCamp()) // 击中自己的队友
                    ) {
                numIgnoreBlocks++;
            }
        } else if (block->getType() == BlockType::MAP_BORDER ||
                block->getType() == BlockType::WALL ||
                block->getType() == BlockType::STONE) {
            showEffect = true;
        } else if (block->getType() == BlockType::BULLET) {
            //printf(">>> [Bullet::onCollidedWith] block id: %d\n", block->id());
            showEffect = true;
        }
    }
    //printf(">>> [Bullet::onCollidedWith] numIgnoreBlocks: %d\n", numIgnoreBlocks);
    //printf(">>> [Bullet::onCollidedWith] withBlocksi.size: %d\n", withBlocks.size());
    if (numIgnoreBlocks > 0 && numIgnoreBlocks == withBlocks.size()) {
        return;
    }

    if (showEffect) {
        _showEffect();
        _playEffectVoice();
    }

    _recycle();
}

const char* Bullet::getVoicePath() const {
    return "music/bin.mp3";
}
