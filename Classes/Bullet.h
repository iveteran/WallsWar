#pragma once

#include "Common.h"
#include "Weapon.h"

class Bullet : public Weapon {
public:
    static std::set<BlockType> CollidingAbleBlockTypes;

public:
    static void initSpriteFrameCache();
    CREATE_FUNC(Bullet);

public:
    bool init() override;
    int getSize() const override { return BULLET_SIZE; }
    BlockType getType() const override { return BlockType::BULLET; }
    std::string getSpriteFrameName() const override { return "bullet"; }

    void setLevel(int level);
    int getLevel() const { return _level; }

    void setBeIntersection(bool value = true);
    bool getBeIntersection() const;

    virtual int getMovingStep() const override;
    std::set<BlockType> getCollidingAbleBTs() const;
    virtual void onBeCollided(Block* activeBlock) override;
    virtual void onCollidedWith(cocos2d::Vector<Block*>& withBlocks) override;

    virtual void changeSpriteDirection() override {}

private:
    void _showEffect();                            // 展示碰撞特效
    void _playEffectVoice();
    const char* getVoicePath() const;

private:
    int _level = 1;                            // 子弹等级
    bool _isBeIntersection = false;
};
