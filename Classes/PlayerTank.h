#pragma once

#include "Block.h"
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

    void createBlock1();
    void createBlock2();
    void createBlock4();
    void choiceCreatingBlockType(RRDirection rrd);

    static void loadFrameAnimation();                          // 加载坦克移动帧动画

protected:
    const cocos2d::Vector<cocos2d::Animate*>* _getAnimations() override;

private:
    bool _beControl = false;
    static cocos2d::Vector<cocos2d::Animate*> _animations[4];   // 存储坦克移动帧动画（方向和等级）

    int _creatingBlock = (int)BlockType::WALL;     // default is BlockType::WALL

    void _initBullets() override;
    bool _isTankIntersection() override;
};
