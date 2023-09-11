#pragma once

#include "cocos2d.h"
#include "Common.h"

using BulletLevel = short;

class Bullet : public cocos2d::Sprite {
public:
    bool init() override;

    void startMove();

    void setDirection(Direction dir);
    void setLevel(BulletLevel lev);

    static void addSpriteFrameCache();

private:
    void _autoMove(float t);
    void _stopMove();
    bool _isMapIntersection();                     // 检测和地图边缘的碰撞
    bool _isBlockIntersection();                   // 检测和方块的碰撞
    virtual bool _isTankIntersection() = 0;        // 检测和坦克的碰撞
    virtual bool _isBulletIntersection() = 0;      // 检测和子弹的碰撞
    void _showEffect();                            // 展示碰撞特效

    Direction _dir{};                                      // 子弹方向
    BulletLevel _level{};                            // 子弹等级
};
