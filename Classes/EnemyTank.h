#pragma once

#include "TankBase.h"

class EnemyTank : public TankBase {
public:
    CREATE_FUNC(EnemyTank);

    bool init() override;

    void setDirection(Direction dir) override;

    static void loadFrameAnimation();                          // 加载坦克移动帧动画
    void changeDirection();                                    // 更改方向
    virtual void disBlood();                                   // 坦克掉血

protected:
    void _initBullets() override;
    const cocos2d::Vector<cocos2d::Animate*>*
        _getAnimations() override;

private:
    static cocos2d::Vector<cocos2d::Animate*> _animations[4];   // 存储坦克移动帧动画（方向和等级）
    bool _canChangeDir = false;                                 // 能否更改方向

    bool _isTankIntersection() override;
};
