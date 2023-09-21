#pragma once

#include "TankBase.h"

class PlayerTank : public TankBase {
public:
    CREATE_FUNC(PlayerTank);

    bool init() override;

    void setDirection(Direction dir) override;
    void shoot() override;

    void setBeControl(bool b = true);
    bool isBeControl();
    void disBlood() override;

    static void loadFrameAnimation();                          // 加载坦克移动帧动画

protected:
    const cocos2d::Vector<cocos2d::Animate*>* _getAnimations() override;

private:
    bool _beControl = false;
    static cocos2d::Vector<cocos2d::Animate*> _animations[4];   // 存储坦克移动帧动画（方向和等级）

    void _initBullets() override;
    bool _isTankIntersection() override;
};
