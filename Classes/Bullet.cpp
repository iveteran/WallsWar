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

    this->setVisible(false);

    MapLayer::getInstance()->addChild(this);

    return true;
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
    mapLayer->addChild(effect);
    effect->setPosition(getPosition());

    effect->runAction(Sequence::create(
        Animate::create(animation),
        CallFunc::create([=]() {
        mapLayer->removeChild(effect);
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
}

void Bullet::onCollidedWith(Vector<Block*>& withBlocks) {
    bool showEffect = false;
    int numIgnoreBlocks = 0;
    for (auto block : withBlocks) {
        if (block->getType() == BlockType::PLAYER) {
            auto player = dynamic_cast<Player*>(block);
            if (_creator && _creator == player || // 击中的是自己
                    _creator->getCamp() == player->getCamp() // 击中自己的队友
                    ) {
                numIgnoreBlocks++;
            }
        }
        if (block->getType() == BlockType::MAP_BORDER ||
                block->getType() == BlockType::WALL ||
                block->getType() == BlockType::STONE) {
            showEffect = true;
        }
    }
    //printf(">>> numIgnoreBlocks: %d\n", numIgnoreBlocks);
    //printf(">>> withBlocksi.size: %d\n", withBlocks.size());
    if (numIgnoreBlocks > 0 && numIgnoreBlocks == withBlocks.size()) {
        return;
    }

    if (showEffect) {
        _showEffect();
        _playEffectVoice();
    }
    setVisible(false);
    stopMove();
}

const char* Bullet::getVoicePath() const {
    return "music/bin.mp3";
}
