#include "EnemyBullet.h"
#include "MapLayer.h"

bool EnemyBullet::init() {
    if (!Bullet::init())
        return false;

    return true;
}

void EnemyBullet::setBeIntersection() {
    _isBeIntersection = true;
}

bool EnemyBullet::_isTankIntersection() {
    auto player1 = MapLayer::getInstance()->getPlayer1();
    if (player1 && this->getBoundingBox().myIntersectsRect(player1->getBoundingBox())) {
        player1->disBlood();
        return true;
    }

    return false;
}

bool EnemyBullet::_isBulletIntersection() {
    if (_isBeIntersection) {
        _isBeIntersection = false;
        return true;
    }

    return false;
}
