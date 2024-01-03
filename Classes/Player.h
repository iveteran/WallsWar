#pragma once

#include "Actor.h"
#include "Bullet.h"
#include "Direction.h"

using cocos2d::Vector;
using cocos2d::Map;
using cocos2d::Animate;

class ActorController;
class User;
class AI;
class UserAgent;
class Camp;
class Campus;
class Block;
class Bullet;
class Weapon;

class Player : public Actor {
public:
    static constexpr float SIZE = Actor::SIZE;
    static constexpr int MAX_MOVING_DISTANCE = 100;

    static std::set<BlockType> CollidingAbleBlockTypes;

public:
    static void initSpriteFrameCache();
    static void loadFrameAnimation();            // 加载移动帧动画
    static Player* create(ActorController* controller=nullptr);

public:
    bool init(ActorController* controller=nullptr);
    BlockType getType() const override { return BlockType::PLAYER; }

    const char* getAvatarImage() const;

    virtual void playAnimate() override;    // 播放动画
    virtual void stopAnimate() override;    // 停止播放动画
    virtual void playFallingAnimate() override;
    virtual std::string getSpriteFrameName() const { return ""; }
    virtual int getMovingStep() const override;

    void birth(const std::string& frameName);
    void beInvincible(int);
    virtual void disBlood();

    bool isHost() const { return _isHost; }
    //void setHost() { _isHost = true; }
    void moveCamaraToCamp();

    bool beControlledByAI(const AI* ai);
    bool beControlledByUser(const User* user);
    bool beControlledByUserAgent(const UserAgent* userAgent);

    //void enter();
    //void exit();
    virtual Camp* getCamp() const override { return _joinedCamp; }

    Campus* createCampus();

    void handleBeAttacked(const Weapon* weapon);

    void addEnemy(const Player* enemy);
    void removeEnemy(int playerId);
    bool isEnemy(int playerId);

    void joinCamp(Camp* camp);
    void exitCamp();
    bool hasTeammates() const;

    void addEnemyCamp(Camp* camp);
    const Map<int, Camp*>& getEnemyCamps() const;
    bool isFreeMan() const;

    ActorController* getController() const { return _controller; }

    void setInitialPosition();
    Direction getInitialDirection() const;
    void setInitialDirection();
    virtual bool setDirection(Direction dir) override;
    void changeEnemyDirection();

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
    void scrollCreatingBlockType(RRDirection rrd);
    void choiceCreatingBlockType(BlockType type);

    std::set<BlockType> getCollidingAbleBTs() const;

    virtual void onBeCollided(Block* activeBlock) override;
    virtual void onCollidedWith(Vector<Block*>& withBlocks) override;

    //virtual void onDead();
    //virtual void onBirth();

private:
    void addBloodRing();
    void addDirectionIndicator();
    void loadSpriteFrames();
    Vec2 calculateDirectionIndicatorPosition();

    void _initBullets();
    void _detachBullets();

    void _shoot(Bullet* bullet);

private:
    Sprite* _bloodRing = nullptr;
    Sprite* _dirIndicator = nullptr;
    time_t _birthTime;
    int _blood = 1;
    bool _isInvincible = false;
    bool _isHost = false;
    bool _isBeControl = false;
    Campus* _selfCampus = nullptr;
    Camp* _joinedCamp = nullptr;
    Map<int, Player*> _enemies;

    ActorController* _controller = nullptr;

    Vector<Bullet*> _bullets;   // 存储坦克所有的子弹

    int _bombs = 4;             // 炮弹
    int _smokeBombs = 2;        // 烟雾弹
    int _flashBombs = 2;        // 照明弹
    int _fireBombs = 2;         // 燃烧弹

    int _creatingBlock = (int)BlockType::WALL;     // default is BlockType::WALL
};
