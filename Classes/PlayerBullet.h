#pragma once

#include "Bullet.h"

class PlayerBullet : public Bullet {
public:
    bool init() override;

    CREATE_FUNC(PlayerBullet);

private:
    bool _isTankIntersection() override;
    bool _isBulletIntersection() override;
};
