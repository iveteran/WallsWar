#include "Common.h"

#include "EnemyTank.h"
#include "EnemyBullet.h"
#include "RandomUtil.h"
#include "MapLayer.h"

USING_NS_CC;

cocos2d::Vector<cocos2d::Animate*> EnemyTank::_animations[4]{};

bool EnemyTank::init() {
    if (!TankBase::init()) {
        return false;
    }

    // 随机选择一个类型
    _level = RandomUtil::random(0, 3);

    // 不断移动
    startMove();

    // 展示出生动画
    birth("enemy_" + std::to_string(int(_dir)) + "_" + std::to_string(_level));

    return true;
}

void EnemyTank::setDirection(Direction dir) {
    if (dir == _dir) {
        return;
    }

    _dir = dir;

    // 当改变方向时，将坐标调整为最接近于8的倍数
    _adjustPosition();

    std::string name = "enemy_" + std::to_string((int)_dir) + "_"
        + std::to_string(_level);

    // 更换图片
    this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
}

void EnemyTank::loadFrameAnimation() {
    auto spriteFrameCache = SpriteFrameCache::getInstance();

    Rect tankRect(0, 0, TANK_SIZE, TANK_SIZE);

    // 总共4个等级
    for (int i = 0; i < 4; i++) {
        std::string lev = std::to_string(i);
        // 总共4个方向
        for (int j = 0; j < 4; j++) {
            std::string dir = std::to_string(j);
            auto enemy_1 = SpriteFrame::create("images/enemy_tank/normal_tank/" + std::to_string(i + 1) + "-" + std::to_string(j + 1) + "-1.png", tankRect);
            auto enemy_2 = SpriteFrame::create("images/enemy_tank/normal_tank/" + std::to_string(i + 1) + "-" + std::to_string(j + 1) + "-2.png", tankRect);
            auto enemy = Animation::createWithSpriteFrames({ enemy_1, enemy_2 }, 0.05f);

            enemy_1->getTexture()->setAliasTexParameters();
            enemy_2->getTexture()->setAliasTexParameters();

            // 添加到缓存
            spriteFrameCache->addSpriteFrame(enemy_1, "enemy_" + dir + "_" + lev);

            // 保存
            _animations[j].pushBack(Animate::create(enemy));
        }
    }
}

void EnemyTank::changeDirection() {
    if (_moveDistance >= MAX_MOVE_DISTANCE) {
        _canChangeDir = true;
    }

    if (!_canChangeDir) return;

    _moveDistance = 0;
    _canChangeDir = false;

    /*if (target == AttacTarget::CAMP) {
        if (RandomUtil::random(1, 10) <= 4) {
            auto camp = MapLayer::getInstance()->getCamp();
            if (this->getPositionX() > camp->getPositionX()) {
                setDirection(Direction::LEFT);
            } else {
                setDirection(Direction::RIGHT);
            }
        } else {
            setDirection(Direction::DOWN);
        }
    }*/

    auto select = RandomUtil::random(1, 10);

    this->stopAnimate();
    if (select <= 4) {
        setDirection(Direction::DOWN);
    } else if (select <= 6) {
        setDirection(Direction::UP);
    } else if (select <= 8) {
        setDirection(Direction::LEFT);
    } else {
        setDirection(Direction::RIGHT);
    }
    this->playAnimate();
}

void EnemyTank::disBlood() {
    TankBase::disBlood();
}

void EnemyTank::_initBullets() {
    auto bullet = EnemyBullet::create();
    _bullets.pushBack(bullet);
}

const cocos2d::Vector<cocos2d::Animate*>* EnemyTank::_getAnimations() {
    return _animations;
}

bool EnemyTank::_isTankIntersection() {
    auto players = MapLayer::getInstance()->getPlayers();
    for (auto player : players) {
        if (this->getBoundingBox().myIntersectsRect(player->getBoundingBox())) {
            return true;
        }
    }

    auto buddies = MapLayer::getInstance()->getEnemies();
    for (auto buddy : buddies) {
        if (buddy != this && this->getBoundingBox().myIntersectsRect(buddy->getBoundingBox())) {
            return true;
        }
    }

    return false;
}
