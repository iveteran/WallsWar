#include "Common.h"

#include "PlayerTank.h"
#include "MapLayer.h"
#include "PlayerBullet.h"
#include "AudioEngine.h"

USING_NS_CC;

cocos2d::Vector<cocos2d::Animate*> PlayerTank::_animations[4]{};

bool PlayerTank::init() {
    if (!TankBase::init()) {
        return false;
    }

    // 玩家出生时血量为3
    _blood = 3;

    // 展示出生动画
    birth("player1_1_" + std::to_string(_level));

    // 玩家出生时方向向上
    _dir = Direction::UP;

    return true;
}

void PlayerTank::setDirection(Direction dir) {
    if (!canMove) return;

    if (dir == _dir) {
        return;
    }

    _dir = dir;

    // 当改变方向时，将坐标调整为最接近于8的倍数
    _adjustPosition();

    std::string name = "player1_" + std::to_string((int)_dir) + "_"
        + std::to_string(_level);

    // 更换图片
    this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
}

void PlayerTank::_initBullets() {
    // 玩家最多拥有两颗子弹
    auto bullet1 = PlayerBullet::create();
    auto bullet2 = PlayerBullet::create();
    _bullets.pushBack(bullet1);
    _bullets.pushBack(bullet2);
}

bool PlayerTank::_isTankIntersection() {
    auto& enemies = MapLayer::getInstance()->getEnemies();

    for (auto enemy : enemies) {
        if (this->getBoundingBox().myIntersectsRect(enemy->getBoundingBox())) {
            return true;
        }
    }

    return false;
}

void PlayerTank::loadFrameAnimation() {
    auto spriteFrameCache = SpriteFrameCache::getInstance();

    Rect tankRect(0, 0, TANK_SIZE, TANK_SIZE);

    // 总共4个等级
    for (int i = 0; i < 4; i++) {
        std::string lev = std::to_string(i);
        // 总共4个方向
        for (int j = 0; j < 4; j++) {
            std::string dir = std::to_string(j);
            auto player1_1 = SpriteFrame::create("images/player1_tank/m" + lev + "-" + dir + "-1.png", tankRect);
            auto player1_2 = SpriteFrame::create("images/player1_tank/m" + lev + "-" + dir + "-2.png", tankRect);
            auto player1 = Animation::createWithSpriteFrames({ player1_1, player1_2 }, 0.05f);

            player1_1->getTexture()->setAliasTexParameters();
            player1_2->getTexture()->setAliasTexParameters();

            // 添加到缓存
            spriteFrameCache->addSpriteFrame(player1_1, "player1_" + dir + "_" + lev);

            // 保存
            _animations[j].pushBack(Animate::create(player1));
        }
    }
}

const cocos2d::Vector<cocos2d::Animate*>* PlayerTank::_getAnimations() {
    return _animations;
}

void PlayerTank::shoot() {
    if (!canMove) {
        return;
    }

    auto bullet1 = _bullets.at(0);
    auto bullet2 = _bullets.at(1);

    if (!bullet1->isVisible() && !bullet2->isVisible()) {
        bullet1->setLevel(_level);
        _shoot(bullet1);
    } else if (bullet1->isVisible() && bullet2->isVisible()) {
        // 什么都不做
    } else if (_level >= 2) {
        // 此时可发射两枚子弹
        if (bullet1->isVisible()) {
            bullet2->setLevel(_level);
            _shoot(bullet2);
        } else {
            bullet1->setLevel(_level);
            _shoot(bullet1);
        }
    }
}

void PlayerTank::setBeControl(bool b) {
    _beControl = b;
}

bool PlayerTank::isBeControl() {
    return _beControl;
}

void PlayerTank::disBlood() {
    if (_isInvincible)
        return;

    auto spriteFrameCache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> spriteFrames;

    for (int i = 0; i != 5; i++) {
        std::string n = std::to_string(i);
        auto spriteFrame = spriteFrameCache->getSpriteFrameByName("blast_" + n);
        spriteFrames.pushBack(spriteFrame);
    }

    // TODO 每次死亡都重新构造动画
    auto blastAnimation = Animation::createWithSpriteFrames(spriteFrames, 0.1f);
    auto blastanimate = Animate::create(blastAnimation);

    // 播放音效
    AudioEngine::play2d("music/enemy-bomb.mp3");

    // 播放动画
    auto node = Sprite::create();
    MapLayer::getInstance()->addChild(node);
    node->setPosition(this->getPosition());
    node->runAction(Sequence::create(blastanimate,
                                     CallFunc::create([node] {node->removeFromParentAndCleanup(true); }),
                                     nullptr));

    if (--_blood == 0) {
        // 移除该坦克
        auto& players = MapLayer::getInstance()->getPlayers();
        players.eraseObject(this);
        this->removeFromParentAndCleanup(true);

        // 显示一个从左到右的动画
        auto gameover = Sprite::create("images/gameover.png");
        gameover->getTexture()->setAliasTexParameters();
        MapLayer::getInstance()->addChild(gameover);
        gameover->setPosition(-gameover->getContentSize().width / 2, PLAYER1_START_Y);
        
        auto moveTo = MoveTo::create(1.0f, { PLAYER1_START_X - 8, PLAYER1_START_Y });
        gameover->runAction(Sequence::create(moveTo, 
                                             DelayTime::create(1.0f),
                                             CallFunc::create([gameover](){gameover->removeFromParent();}), 
                                             nullptr));
    } else {
        // 播放动画
        _isInvincible = true; // 防止掉血
        // 回到出生点
        birth("player1_" + std::to_string((int)_dir) + "_" + std::to_string(_level));
    }
}
