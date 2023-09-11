#pragma once

#include "Bullet.h"

class EnemyBullet : public Bullet {
public:
    bool init() override;

    CREATE_FUNC(EnemyBullet);
    void setBeIntersection();

private:
    bool _isTankIntersection() override;
    bool _isBulletIntersection() override;

    bool _isBeIntersection = false;
};
