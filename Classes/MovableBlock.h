#pragma once

#include "Block.h"
#include "Direction.h"

using cocos2d::Vector;
using cocos2d::Vec2;

enum class PositionType {
    FLOOR,
    POSITION,
};

bool isActor(BlockType bType);
float calculateRotateDegree(Direction from, Direction to);

class MovableBlock : public Block {
public:
    static std::set<BlockType> CollidingAbleBlockTypes;

public:
    bool init() override;
    BlockType getType() const override { return BlockType::MOVABLE_BLOCK; }

    bool movable() const override { return true; }
    int getDistances() const { return _distances; }

    virtual Direction getDirection() const { return _dir; }
    virtual bool setDirection(Direction dir);

    void goBack();
    void moveTo(const Vec2& to);
    void moveTo(float x, float y);
    void moveBy(const Vec2& step);
    void moveBy(float xStep, float yStep);
    void xMoveBy(float step);
    void yMoveBy(float step);
    void setFloor(int floor);
    void increaseFloor();
    void decreaseFloor();

    void startMove(Direction dir=Direction::NONE);      // 开启自动移动
    void stopMove();                                    // 停止自动移动

    void fallDownIfDownFloorIsEmpty();

    virtual bool allowedCameraFollows() const { return _allowedCameraFollows; }
    virtual void allowCameraFollows() { _allowedCameraFollows = true; }
    void makeCameraFollows();

    virtual int getMaxMovingDistance() const { return -1; }
    virtual void onStopped() { }
    virtual bool isNeedRotateForDirection() const { return false; }
    virtual void changeDirection(float degree);
    virtual void playAnimate();                         // 播放移动动画
    virtual void playFallingAnimate();
    virtual void stopAnimate();                         // 停止播放动画

    virtual bool movingMusicEnabled() { return false; }
    virtual int getMovingStep() const { return 1; }

    virtual std::set<BlockType> getCollidingAbleBTs() const;

    bool handleCollisionDetection();
    virtual bool detectCollision(Vector<Block*>* collisionBlocks=nullptr) const;
    bool isCollidedWithMapBorder(const Vector<Block*>& collisionBlocks) const;

    Direction getFromDirection() const { return _fromDir; }
    int getFromFloor() const { return _fromFloor; }
    const Vec2& getFromPosition() const { return _fromPosition; }

    void onMoved();

protected:
    virtual void update(float dt) override;
    void _adjustPosition();
    float _adjustNumber(int number);

    bool _updateBlockManagementPosition(PositionType posOrFloor);
    void _rollbackPosition();
    void _rollbackFloor();

protected:
    using Node::setPosition;

protected:
    bool _movable = true;
    bool _canMove = true;
    bool _isMoving = false;
    int _moveDistance = 0;

    Direction _dir = Direction::UP;

    Direction _fromDir;
    int _fromFloor = 0;
    Vec2 _fromPosition;

    //int _step = 1;
    int _distances = -1;
    int _musicId = -1;

    bool _allowedCameraFollows = false;
};
