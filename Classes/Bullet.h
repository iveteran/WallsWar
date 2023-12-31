#pragma once

#include "Weapon.h"

class Bullet : public Weapon {
public:
    static constexpr float SIZE = 4;
    static constexpr float MAX_MOVING_DISTANCE = 200;
    static std::set<BlockType> CollidingAbleBlockTypes;

public:
    static void initSpriteFrameCache();
    CREATE_FUNC(Bullet);

public:
    bool init() override;
    void destroy();
    int getSize() const override { return SIZE; }
    BlockType getType() const override { return BlockType::BULLET; }
    std::string getSpriteFrameName() const override { return "bullet"; }

    void setLevel(int level);
    int getLevel() const { return _level; }

    void shoot(const Vec2& startPos, Direction dir, int floor);
    bool isFiring() const { return isVisible(); }
    void setFiring(bool yes) { setVisible(yes); }

    void setBeIntersection(bool value = true);
    bool getBeIntersection() const;

    virtual int getMaxMovingDistance() const override { return MAX_MOVING_DISTANCE; }
    virtual void onStopped() override;
    virtual bool isNeedRotateForDirection() const override { return true; }
    virtual int getMovingStep() const override;
    std::set<BlockType> getCollidingAbleBTs() const;
    virtual void onBeCollided(Block* activeBlock) override;
    virtual void onCollidedWith(cocos2d::Vector<Block*>& withBlocks) override;

private:
    void stopAndRecycle();
    void _recycle();
    void _showEffect();                            // 展示碰撞特效
    void _playEffectVoice();
    const char* getVoicePath() const;

private:
    int _level = 1;                            // 子弹等级
    bool _isBeIntersection = false;
};
