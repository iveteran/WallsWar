#pragma once

#include "Gamer.h"
#include "Bullet.h"

using cocos2d::Vector;
using cocos2d::Animate;

class Bullet;
//class Controller;
class AI;
class User;
class UserAgent;
class Camp;
class Campus;
class Block;

class Player : public Gamer {
public:
    static std::set<BlockType> CollidingAbleBlockTypes;
    static bool IsHost;

private:
    // 存储坦克移动帧动画（方向和等级）
    static Vector<Animate*> _animations[4];
    static Vector<Animate*> _enemy_animations[4];

public:
    static void initSpriteFrameCache();
    static void loadFrameAnimation();            // 加载坦克移动帧动画
    static void _loadFrameAnimation(const char* imgPath, const char* namePrefix);
    CREATE_FUNC(Player);

public:
    bool init() override;
    BlockType getType() const override { return BlockType::PLAYER; }

    virtual void playAnimate() override;                         // 播放移动动画
    virtual void playFallingAnimate() override;
    virtual void stopAnimate() override;                         // 停止播放动画
    virtual std::string getSpriteFrameName() const;
    virtual int getMovingStep() const override;

    void birth(const std::string& frameName);           // 坦克出生动画
    void beInvincible(int);                             // 使坦克无敌
    virtual void disBlood();                            // 坦克掉血

    bool isHost() const { return _isHost; }
    void setHost() { _isHost = true; }
    void moveCamaraToCamp();

    bool beControlledByAI(const AI* ai);
    bool beControlledByUser(const User* user);
    bool beControlledByUserAgent(const UserAgent* userAgent);

    //void enter();
    //void exit();
    virtual Camp* getCamp() const override { return _joinedCamp; }

    void joinCamp(Camp* camp);
    void exitCamp();

    void setEnemyCamp(Camp* camp) { _enemyCamp = camp; setInitialDirection(); }
    Camp* getEnemyCamp() const { return _enemyCamp; }
    //Campus* createCampus();

    //Controller* getController() const;

    void setInitialPosition();
    Direction getInitialDirection() const;
    void setInitialDirection();
    void changeDirection();

    bool isBeControl() const { return _isBeControl; }
    void shoot();

    void dropBomb();
    void dropSmokeBomb();
    void dropFlashBomb();
    void dropFireBomb();

    bool createBlock1();
    bool createBlock1(Direction dir);
    bool createBlock2();
    bool createBlock2(Direction dir);
    bool createBlock4();
    void choiceCreatingBlockType(RRDirection rrd);

    std::set<BlockType> getCollidingAbleBTs() const;

    virtual void onBeCollided(Block* activeBlock) override;
    virtual void onCollidedWith(Vector<Block*>& withBlocks) override;

    //virtual void onDead();
    //virtual void onBirth();

private:
    bool _initPlayer();
    bool _initEnemy();

    void _initBullets();
    void _detachBullets();

    void _shoot(Bullet* bullet);

private:
    time_t _birthTime;
    int _blood = 1;
    bool _isInvincible = false;
    bool _isHost = false;
    bool _isBeControl = false;
    //Campus* _campus = nullptr;
    Camp* _joinedCamp = nullptr;
    Camp* _enemyCamp = nullptr;
    //Client* _client;

    Vector<Bullet*> _bullets;   // 存储坦克所有的子弹

    int _bombs = 4;             // 炮弹
    int _smokeBombs = 2;        // 烟雾弹
    int _flashBombs = 2;        // 照明弹
    int _fireBombs = 2;         // 燃烧弹

    int _creatingBlock = (int)BlockType::WALL;     // default is BlockType::WALL
};
