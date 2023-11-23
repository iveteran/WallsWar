#include "AudioEngine.h"

#include "MovableBlock.h"
#include "MapLayer.h"
#include "GameScene.h"
#include "ControlLayer.h"

USING_NS_CC;

std::set<BlockType>
MovableBlock::CollidingAbleBlockTypes{
    BlockType::MAP_BORDER
};

static const int minBordGrap = TANK_SIZE * 2;

std::set<BlockType>
MovableBlock::getCollidingAbleBTs() const {
    return MovableBlock::CollidingAbleBlockTypes;
}

void MovableBlock::changeSpriteDirection() {
    auto name = getSpriteFrameName();
    // 更换图片
    this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
}

void MovableBlock::setDirection(Direction dir) {
    if (dir == _dir) {
        return;
    }
    _fromDir = _dir;
    _dir = dir;

    changeSpriteDirection();
}

void MovableBlock::goBack() {
    //auto pos = getPosition();
    //printf("<<<< goBack: (%f, %f) -> (%f, %f)\n", pos.x, pos.y, _fromPosition.x, _fromPosition.y);
    moveTo(_fromPosition);
}

void MovableBlock::moveTo(const Vec2& to) {
    moveTo(to.x, to.y);
}

void MovableBlock::moveTo(float x, float y) {
    auto pos = getPosition();
    if (abs(pos.x - x) < 0.00001 && abs(pos.y - y) < 0.00001) {
        return;
    }
    _fromPosition = pos;
    setPosition(x, y);
    //printf("<<<< moveTo: (%f, %f) -> (%f, %f)\n", pos.x, pos.y, x, y);
    _updateBlockManagementPosition(PositionType::POSITION);
}

void MovableBlock::moveBy(const Vec2& step) {
    moveBy(step.x, step.y);
}

void MovableBlock::moveBy(float xStep, float yStep) {
    if (abs(xStep) < 0.00001 && abs(yStep) < 0.00001) {
        return;
    }
    _fromPosition = getPosition();
    setPosition(_fromPosition.x + xStep, _fromPosition.y + yStep);
    _updateBlockManagementPosition(PositionType::POSITION);
}

void MovableBlock::xMoveBy(float step) {
    if (abs(step) < 0.00001) {
        return;
    }
    _fromPosition = getPosition();
    setPositionX(_fromPosition.x + step);
    _updateBlockManagementPosition(PositionType::POSITION);
}

void MovableBlock::yMoveBy(float step) {
    if (abs(step) < 0.00001) {
        return;
    }
    _fromPosition = getPosition();
    setPositionY(_fromPosition.y + step);
    _updateBlockManagementPosition(PositionType::POSITION);
}

void MovableBlock::setFloor(int floor) {
    if (floor == getFloor()) {
        return;
    }
    _fromFloor = getFloor();
    Block::setFloor(floor);
    _updateBlockManagementPosition(PositionType::FLOOR);
    CCLOG(">> [MovableBlock::setFloor] block floor change to: %d", getFloor());
}

void MovableBlock::increaseFloor() {
    _fromFloor = getFloor();
    Block::increaseFloor();
    _updateBlockManagementPosition(PositionType::FLOOR);
    CCLOG(">> [MovableBlock::increaseFloor] block floor change to: %d", getFloor());
}

void MovableBlock::decreaseFloor() {
    _fromFloor = getFloor();
    Block::decreaseFloor();
    _updateBlockManagementPosition(PositionType::FLOOR);
    CCLOG(">> [MovableBlock::decreaseFloor] block floor change to: %d", getFloor());
}

void MovableBlock::_adjustPosition() {
    moveTo(_adjustNumber(int(getPositionX())), _adjustNumber(int(getPositionY())));
}

bool MovableBlock::_updateBlockManagementPosition(PositionType posOrFloor) {
    bool success = MapLayer::getInstance()->updateBlockPosition(this);
    if (!success) {
        // 遇到障碍物，处理碰撞检测
        detectCollision();
    }
    return success;
}

void MovableBlock::_rollbackPosition() {
    Vec2 tmpPos = getPosition();
    setPosition(_fromPosition);
    _fromPosition = tmpPos;
}

void MovableBlock::_rollbackFloor() {
    int tmpFloor = getFloor();
    setFloor(_fromFloor);
    _fromFloor = tmpFloor;
}

float MovableBlock::_adjustNumber(int number) {
    //int adjustSize = BLOCK_SIZE;
    int adjustSize = BLOCK_SIZE / 4;
    int mod = number % (int)adjustSize;
    int half_block_size = adjustSize / 2;
    if (mod < half_block_size) {
        number += mod;
    } else if (mod >= half_block_size) {
        number -= adjustSize - mod;
    }

    return float(number);
}

void MovableBlock::startMove(Direction dir) {
    if (_isMoving) return;

    if (dir != Direction::NONE) {
        setDirection(dir);
    }
    if (movingMusicEnabled()) {
        _musicId = AudioEngine::play2d("music/player_move.mp3");
    }
    schedule(CC_SCHEDULE_SELECTOR(MovableBlock::_autoMove), 0);  // 每帧调用
    _isMoving = true;
    playAnimate();
}

void MovableBlock::stopMove() {
    unschedule(CC_SCHEDULE_SELECTOR(MovableBlock::_autoMove));
    if (_musicId != -1) {
        AudioEngine::stop(_musicId);
    }
    // 到达目的地后，将坐标调整为最接近于BLOCK_SIZE的倍数
    //_adjustPosition();
    _isMoving = false;
    stopAnimate();
}

void MovableBlock::_autoMove(float dt) {
    if (!_canMove) {
        return;
    }
    int step = getMovingStep();

    // 假设可以移动
    switch (_dir) {
    case Direction::LEFT:
        xMoveBy(step * -1);
        break;
    case Direction::UP:
        yMoveBy(step);
        break;
    case Direction::RIGHT:
        xMoveBy(step);
        break;
    case Direction::DOWN:
        yMoveBy(step * -1);
        break;
    default:
        break;
    }
    _moveDistance += int(step);

    // 如果产生碰撞，则回到移动之前的位置
    bool occurred = handleCollisionDetection();
    if (!occurred) {
        onMoved();
    }
}

void MovableBlock::onMoved() {
    // 如果玩家所在楼层大于1，在移动后楼下层为空就使其下落
    fallDownIfDownFloorIsEmpty();

    if (allowedCameraFollows())
    {
        // 如果是玩家移动，让摄像头跟随他
        makeCameraFollows();
    }
}

bool MovableBlock::handleCollisionDetection() {
    Vector<Block*> collidedBlocks;
    bool occurred = detectCollision(&collidedBlocks);
    if (occurred) {
        if (isCollidedWithMapBorder(collidedBlocks)) {
            goBack();
        } else {
            // 调用所有被碰撞者的回调函数，参数是碰撞者(主动碰撞)
            for (auto block : collidedBlocks) {
                block->onBeCollided(this);
            }
        }
        // 调用自己的碰撞回调函数，参数是所有被碰撞者(被动者)
        onCollidedWith(collidedBlocks);
    }
    return occurred;
}

bool MovableBlock::detectCollision(Vector<Block*>* collisionBlocks) const {
    return isMapBorderIntersection(collisionBlocks) ||
        isBlockIntersection(getFloor(), collisionBlocks);
}

// 碰到地图边缘
bool MovableBlock::isCollidedWithMapBorder(const Vector<Block*>& collisionBlocks) const {
    return !collisionBlocks.empty() &&
        collisionBlocks.at(0)->getType() == BlockType::MAP_BORDER;
}

void MovableBlock::playAnimate() {
}

void MovableBlock::playFallingAnimate() {
}

void MovableBlock::stopAnimate() {
}

// 如果Floor大于0，在移动时要检查下一层是否空的，如果是空的就使其掉落到下一层直至到地面(0层)
void MovableBlock::fallDownIfDownFloorIsEmpty() {
    while (getFloor() > 0 && isDownFloorEmpty()) {
        decreaseFloor();
        CCLOG(">> [MovableBlock::fallDownIfDownFloorIsEmpty] Player fall to floor: %d", getFloor());
        playFallingAnimate();
    }
}

void MovableBlock::makeCameraFollows() {
    auto camera = Camera::getDefaultCamera();
    auto cameraPos = camera->getPosition();

    Size visible_size = Director::getInstance()->getVisibleSize();
    auto pos = getPosition();

    int xOffset = 0, yOffset = 0;
    int gap = 0;
    float cameraGap = 0;
    switch (_dir)
    {
        case Direction::LEFT:
            cameraGap = cameraPos.x - visible_size.width / 2 + (CENTER_WIDTH / 2 - visible_size.width / 2);
            gap = pos.x - cameraGap;
            if (gap < minBordGrap && cameraGap > 0)
            {
                xOffset = -1;
            }
            break;
        case Direction::RIGHT:
            gap = visible_size.width - pos.x;
            if (cameraPos.x < CENTER_WIDTH / 2 && gap < minBordGrap)
            {
                xOffset = 1;
            }
            break;
        case Direction::UP:
            gap = visible_size.height - pos.y;
            if (cameraPos.y < CENTER_HEIGHT / 2 && gap < minBordGrap)
            {
                yOffset = 1;
            }
            break;
        case Direction::DOWN:
            cameraGap = cameraPos.y - visible_size.height / 2 + (CENTER_HEIGHT / 2 - visible_size.height / 2);
            gap = pos.y - cameraGap;
            if (gap < minBordGrap && cameraGap > 0)
            {
                yOffset = -1;
            }
            break;
        default:
            break;
    }
    if (xOffset != 0 || yOffset != 0) {
        cameraPos.x += xOffset;
        cameraPos.y += yOffset;
        camera->setPosition(cameraPos);

        // 使ControlLayer跟随摄像头，从而固定在屏幕相应位置
        auto ctrlLayer = GET_CONTROL_LAYER();
        if (ctrlLayer != nullptr) {
            auto ctrlLayerPos = ctrlLayer->getPosition();
            ctrlLayer->setPosition(ctrlLayerPos.x + xOffset, ctrlLayerPos.y + yOffset);
        }
    }
}
