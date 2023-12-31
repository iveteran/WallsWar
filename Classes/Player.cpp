#include "Player.h"
#include "MapLayer.h"
#include "ControlLayer.h"
#include "GameScene.h"
#include "Bullet.h"
#include "Camp.h"
#include "constants/CampConstants.h"

#include "RandomUtil.h"
#include "AudioEngine.h"

USING_NS_CC;

bool Player::IsHost = false;
Vector<Animate*> Player::_animations[4]{};
Vector<Animate*> Player::_enemy_animations[4]{};

std::set<BlockType>
Player::CollidingAbleBlockTypes{
    BlockType::MAP_BORDER,
    BlockType::PLAYER,
    BlockType::BULLET,
    BlockType::WALL,
    BlockType::STONE,
    BlockType::ICE,
    BlockType::RIVER,
    BlockType::TRENCH,
    BlockType::NPC,
    BlockType::CAMP
};

static const float movingMinBoundaryGap = Player::SIZE * 5;

Vec2 convertToMapPosition(const Vec2& pos);

std::set<BlockType>
Player::getCollidingAbleBTs() const {
    return Player::CollidingAbleBlockTypes;
}

void Player::initSpriteFrameCache() {
    auto spriteFrameCache = SpriteFrameCache::getInstance();

    // 坦克爆炸帧动画
    auto* blast_0 = Sprite::create("images/blast/0.png")->getSpriteFrame();
    auto* blast_1 = Sprite::create("images/blast/1.png")->getSpriteFrame();
    auto* blast_2 = Sprite::create("images/blast/2.png")->getSpriteFrame();
    auto* blast_3 = Sprite::create("images/blast/3.png")->getSpriteFrame();
    auto* blast_4 = Sprite::create("images/blast/4.png")->getSpriteFrame();

    blast_0->getTexture()->setAliasTexParameters();
    blast_1->getTexture()->setAliasTexParameters();
    blast_2->getTexture()->setAliasTexParameters();
    blast_3->getTexture()->setAliasTexParameters();
    blast_4->getTexture()->setAliasTexParameters();

    spriteFrameCache->addSpriteFrame(blast_0, "blast_0");
    spriteFrameCache->addSpriteFrame(blast_1, "blast_1");
    spriteFrameCache->addSpriteFrame(blast_2, "blast_2");
    spriteFrameCache->addSpriteFrame(blast_3, "blast_3");
    spriteFrameCache->addSpriteFrame(blast_4, "blast_4");

    // 坦克出生前的星星帧动画
    auto star_0 = Sprite::create("images/star0.png")->getSpriteFrame();
    auto star_1 = Sprite::create("images/star1.png")->getSpriteFrame();
    auto star_2 = Sprite::create("images/star2.png")->getSpriteFrame();
    auto star_3 = Sprite::create("images/star3.png")->getSpriteFrame();

    star_0->getTexture()->setAliasTexParameters();
    star_1->getTexture()->setAliasTexParameters();
    star_2->getTexture()->setAliasTexParameters();
    star_3->getTexture()->setAliasTexParameters();

    spriteFrameCache->addSpriteFrame(star_0, "star_0");
    spriteFrameCache->addSpriteFrame(star_1, "star_1");
    spriteFrameCache->addSpriteFrame(star_2, "star_2");
    spriteFrameCache->addSpriteFrame(star_3, "star_3");

    // 坦克保护环帧动画
    auto ring_0 = Sprite::create("images/ring0.png")->getSpriteFrame();
    auto ring_1 = Sprite::create("images/ring1.png")->getSpriteFrame();

    ring_0->getTexture()->setAliasTexParameters();
    ring_1->getTexture()->setAliasTexParameters();

    spriteFrameCache->addSpriteFrame(ring_0, "ring_0");
    spriteFrameCache->addSpriteFrame(ring_1, "ring_1");
}

void Player::loadFrameAnimation() {
    const char* playerImgPath = "images/player1_tank";
    const char* playerNamePrefix = "player1";
    _loadFrameAnimation(playerImgPath, playerNamePrefix);

    const char* enemyImgPath = "images/enemy_tank/normal_tank";
    const char* enemyNamePrefix = "enemy";
    _loadFrameAnimation(enemyImgPath, enemyNamePrefix);
}

void Player::_loadFrameAnimation(const char* imgPath, const char* namePrefix) {
    auto spriteFrameCache = SpriteFrameCache::getInstance();

    Rect playerRect(0, 0, Player::SIZE, Player::SIZE);
    bool isHost = strcmp(namePrefix, "player1") == 0;

    // 总共4个等级，从0到3
    for (int level = 0; level < 4; level++) {
        // 总共4个方向：left(0), up(1), right(2), down(3)
        for (int dir = (int)Direction::NONE+1; dir < (int)Direction::COUNT; dir++) {
            char buf[128] = {0};
            // FIXME: 修改图片名称使其统一
            if (isHost) {
                snprintf(buf, sizeof(buf), "%s/m%d-%d-1.png", imgPath, level, dir);
            } else { // for enemy
                snprintf(buf, sizeof(buf), "%s/%d-%d-1.png", imgPath, level+1, dir+1);
            }
            auto player_1 = SpriteFrame::create(buf, playerRect);
            // FIXME: 修改图片名称使其统一
            if (isHost) {
                snprintf(buf, sizeof(buf), "%s/m%d-%d-2.png", imgPath, level, dir);
            } else { // for enemy
                snprintf(buf, sizeof(buf), "%s/%d-%d-2.png", imgPath, level+1, dir+1);
            }
            auto player_2 = SpriteFrame::create(buf, playerRect);
            auto player = Animation::createWithSpriteFrames({ player_1, player_2 }, 0.05f);

            player_1->getTexture()->setAliasTexParameters();
            player_2->getTexture()->setAliasTexParameters();

            // 添加到缓存
            char name[128] = {0};
            snprintf(name, sizeof(name), "%s_%d_%d", namePrefix, dir, level);
            spriteFrameCache->addSpriteFrame(player_1, name);

            // 保存
            if (isHost) {
                _animations[dir].pushBack(Animate::create(player));
            } else {
                _enemy_animations[dir].pushBack(Animate::create(player));
            }
        }
    }
}

const char* Player::getAvatar() const {
    if (_isHost) {
        return "images/yuu-avatar.jpg";
    } else {
        return "images/game-manager.png";
    }
}

bool Player::init() {
    if (!Actor::init()) {
        return false;
    }
    _canMove = false;

    _initBullets();

    _isHost = Player::IsHost;
    if (_isHost) {
        return _initPlayer();
    } else {
        return _initEnemy();
    }

    return true;
}

bool Player::_initPlayer() {
    // 玩家出生时血量为3
    _blood = 3;

    // 展示出生动画
    char name[128] = {0};
    snprintf(name, sizeof(name), "player1_%d_%d", (int)_dir, _level);
    birth(name);

    return true;
}

bool Player::_initEnemy() {
    // 随机选择一个类型
    _level = RandomUtil::random(0, 3);

    // 展示出生动画
    char name[128] = {0};
    snprintf(name, sizeof(name), "enemy_%d_%d", (int)_dir, _level);
    birth(name);

    return true;
}

std::string Player::getSpriteFrameName() const {
    char name[128] = {0};
    if (_isHost) {
        snprintf(name, sizeof(name), "player1_%d_%d", (int)_dir, _level);
    } else {
        snprintf(name, sizeof(name), "enemy_%d_%d", (int)_dir, _level);
    }
    return std::string(name);
}

Direction Player::getInitialDirection() const {
    Direction dir = Direction::NONE;
    auto campPos = _joinedCamp->getPosition();
    auto enemyCampPos = _enemyCamp->getPosition();
    if (campPos.x > enemyCampPos.x) {
        dir = Direction::LEFT;
        int xDistance = campPos.x - enemyCampPos.x;
        if (campPos.y > enemyCampPos.y) {
            int yDistance = campPos.y - enemyCampPos.y;
            if (yDistance < xDistance) {
                dir = Direction::DOWN;
            }
        }
    } else {
        dir = Direction::RIGHT;
        int xDistance = abs(campPos.x - enemyCampPos.x);
        if (campPos.y < enemyCampPos.y) {
            int yDistance = abs(campPos.y - enemyCampPos.y);
            if (yDistance < xDistance) {
                dir = Direction::UP;
            }
        }
    }
    return dir;
}

void Player::setInitialDirection() {
    Direction dir = getInitialDirection();
    setDirection(dir);
}

void Player::setInitialPosition() {
    bool done = false;
    auto campPos = _joinedCamp->getPosition();
    int offset = Camp::SIZE / 2 + BLOCK_SIZE + Player::SIZE / 2;  // 营地大小的一半+围墙大小+玩家大小的一半
    // Camp的上边
    Vec2 pos1 = Vec2(campPos.x, campPos.y + offset);
    // Camp的右上
    Vec2 pos2 = Vec2(campPos.x + offset, campPos.y + offset);
    // Camp的右边
    Vec2 pos3 = Vec2(campPos.x + offset, campPos.y);
    // Camp的下边
    Vec2 pos4 = Vec2(campPos.x, campPos.y - offset);
    // Camp的左下
    Vec2 pos5 = Vec2(campPos.x - offset, campPos.y - offset);
    // Camp的左边
    Vec2 pos6 = Vec2(campPos.x - offset, campPos.y);

    std::vector<Vec2> candidatePositions{pos1, pos2, pos3, pos4, pos5, pos6};
    for (auto pos : candidatePositions) {
        moveTo(pos);
        //printf(">>> player pos: (%f, %f)\n", pos.x, pos.y);
        if (!detectCollision()) {
            done = true;
            break;
        }
    }
    if (!done) {
        // to try next second later
        //scheduleOnce(CC_SCHEDULE_SELECTOR(Player::setInitialPosition), 1.0f);
    }
}

void Player::_initBullets() {
    // 玩家最多拥有两颗子弹
    auto bullet1 = Bullet::create();
    bullet1->setCreator(this);
    _bullets.pushBack(bullet1);

    auto bullet2 = Bullet::create();
    bullet2->setCreator(this);
    _bullets.pushBack(bullet2);
}

void Player::_detachBullets() {
    for (auto bullet : _bullets) {
        if (bullet->isFiring()) {
            bullet->unsetCreator();
        } else {
            bullet->destroy();
        }
    }
}

void Player::playAnimate() {
    if (!_canMove) {
        return;
    }

    if (_isHost) {
        this->runAction(RepeatForever::create(_animations[int(_dir)].at(_level)));
    } else {
        this->runAction(RepeatForever::create(_enemy_animations[int(_dir)].at(_level)));
    }
}

void Player::playFallingAnimate() {
    // rising, falling
    //this->runAction(RepeatForever::create(_animations[int(_dir)].at(_level)));
}

void Player::stopAnimate() {
    if (!_canMove) {
        return;
    }

    this->stopAllActions();
}

int Player::getMovingStep() const {
    int step = MovableBlock::getMovingStep() + _level * 0.2f;
    return step;
}

void Player::birth(const std::string& frameName) {
    if (_isHost)
    {
        _level = 0;
    }
    _canMove = false;
    stopAllActions();

    auto spriteFrameCache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> spriteFrames;
    for (int i = 0; i < 4 * 2; i++) {
        std::string n = std::to_string(i % 4);
        auto spriteFrame = spriteFrameCache->getSpriteFrameByName("star_" + n);
        spriteFrames.pushBack(spriteFrame);
    }

    // 星星动画
    auto animation = Animation::createWithSpriteFrames(spriteFrames, 0.2f);
    auto animate = Animate::create(animation);

    auto action = this->runAction(Sequence::create(
        animate,
        CallFunc::create([=]() {
            this->initWithSpriteFrameName(frameName);
            _canMove = true;
            if (_isHost) {
                this->beInvincible(3);
                moveCamaraToCamp();
            }
        }),
        nullptr
    ));
}

void Player::moveCamaraToCamp() {
    auto camera = Camera::getDefaultCamera();
    auto originCameraPos = camera->getPosition();
    printf(">> camera pos: (%f, %f)\n", originCameraPos.x, originCameraPos.y);
    auto cameraPos = convertToMapPosition(originCameraPos);
    printf(">> camera pos for map: (%f, %f)\n", cameraPos.x, cameraPos.y);

    auto campPos = _joinedCamp->getPosition();
    printf(">> camp pos: (%f, %f)\n", campPos.x, campPos.y);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    float xOffset = 0, yOffset = 0;
    if (cameraPos.x > campPos.x) {
        xOffset = cameraPos.x - campPos.x - visibleSize.width / 2 + Camp::SIZE  / 2;
        xOffset += std::min(campPos.x - Camp::SIZE / 2, movingMinBoundaryGap);
        xOffset = xOffset > 0 ? xOffset * -1 : 0;
    } else {
        xOffset = campPos.x - cameraPos.x - visibleSize.width / 2 + Camp::SIZE  / 2;
        xOffset += std::min(CENTER_WIDTH - campPos.x - Camp::SIZE / 2, movingMinBoundaryGap);
        xOffset = xOffset > 0 ? xOffset : 0;
    }
    if (cameraPos.y > campPos.y) {
        yOffset = cameraPos.y - campPos.y - visibleSize.height / 2 + Camp::SIZE / 2;
        yOffset += std::min(campPos.y - Camp::SIZE / 2, movingMinBoundaryGap);
        yOffset = yOffset > 0 ? yOffset * -1 : 0;
    } else {
        yOffset = campPos.y - cameraPos.y - visibleSize.height / 2 + Camp::SIZE / 2;
        yOffset += std::min(CENTER_HEIGHT - campPos.y - Camp::SIZE / 2, movingMinBoundaryGap);
        yOffset = yOffset > 0 ? yOffset : 0;
    }

    printf(">> offset: (%f, %f)\n", xOffset, yOffset);
    if (xOffset != 0 || yOffset != 0 ) {
        // make moving effect
        float movingTime = std::max(abs(xOffset), abs(yOffset)) * 0.01f;
        movingTime = std::min(movingTime, 3.0f);
        printf(">> movingTime: %f\n", movingTime);

        camera->runAction(MoveBy::create(movingTime, Vec2(xOffset, yOffset)));

        auto ctrlLayer = GET_CONTROL_LAYER();
        if (ctrlLayer != nullptr) {
            ctrlLayer->runAction(MoveBy::create(movingTime, Vec2(xOffset, yOffset)));
        }

        CCLOG(">> [moveCamaraToCamp] camera move to position: (%f, %f)",
                camera->getPosition().x, camera->getPosition().y);
    }
}

void Player::beInvincible(int time) {
    _isInvincible = true;
    auto ring = Sprite::create();
    auto spriteFrameCache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> spriteFrames;
    for (int i = 0; i < 2 * time * 5; i++) {
        std::string n = std::to_string(i % 2);
        auto spriteFrame = spriteFrameCache->getSpriteFrameByName("ring_" + n);
        spriteFrames.pushBack(spriteFrame);
    }
    auto animation = Animation::createWithSpriteFrames(spriteFrames, 0.1f);
    auto animate = Animate::create(animation);
    addChild(ring);
    ring->setPosition(this->getContentSize() / 2);
    ring->runAction(Sequence::create(
        animate,
        CallFunc::create([=]() {
        ring->removeFromParent();
        _isInvincible = false;
    })
        , nullptr
        ));
}

void Player::changeDirection() {
    if (_moveDistance < Player::MAX_MOVE_DISTANCE) {
        return;
    }
    _moveDistance = 0;

    auto select = RandomUtil::random(1, 10);
    this->stopAnimate();
    if (select <= 4) {
        setDirection(Direction::DOWN);
    } else if (select <= 6) {
        setDirection(Direction::UP);
    } else if (select <= 8) {
        setDirection(Direction::LEFT);
    } else {
        setDirection(Direction::RIGHT);
    }
    this->playAnimate();
}

void Player::shoot() {
    if (!_canMove) {
        return;
    }

    auto bullet1 = _bullets.at(0);
    auto bullet2 = _bullets.at(1);

    if (!bullet1->isFiring() && !bullet2->isFiring()) {
        _shoot(bullet1);
    } else if (bullet1->isFiring() && bullet2->isFiring()) {
        // 什么都不做
    } else if (_level >= 2) {
        // 此时可发射两枚子弹
        if (bullet1->isFiring()) {
            _shoot(bullet2);
        } else {
            _shoot(bullet1);
        }
    }
}

void Player::_shoot(Bullet* bullet) {
    if (_isHost)
        AudioEngine::play2d("music/shoot.mp3");

    Vec2 startPos;
    auto playerPos = getPosition();
    switch (_dir) {
    case Direction::LEFT:
        startPos = Vec2(playerPos.x - Player::SIZE / 2.0f, playerPos.y);
        break;
    case Direction::UP:
        startPos = Vec2(playerPos.x, playerPos.y + Player::SIZE / 2.0f);
        break;
    case Direction::RIGHT:
        startPos = Vec2(playerPos.x + Player::SIZE / 2.0f, playerPos.y);
        break;
    case Direction::DOWN:
        startPos = Vec2(playerPos.x, playerPos.y - Player::SIZE / 2.0f);
        break;
    default:
        break;
    }

    bullet->shoot(startPos, _dir, getFloor());
}

void Player::disBlood() {
    if (_isInvincible)
        return;

    auto spriteFrameCache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> spriteFrames;

    for (int i = 0; i != 5; i++) {
        std::string n = std::to_string(i);
        auto spriteFrame = spriteFrameCache->getSpriteFrameByName("blast_" + n);
        spriteFrames.pushBack(spriteFrame);
    }

    // TODO 每次死亡都重新构造动画
    auto blastAnimation = Animation::createWithSpriteFrames(spriteFrames, 0.1f);
    auto blastanimate = Animate::create(blastAnimation);

    // 播放动画
    auto mapLayer = MapLayer::getInstance();
    auto node = Sprite::create();
    mapLayer->addNode(node);
    node->setPosition(getPosition());
    node->runAction(Sequence::create(blastanimate,
                CallFunc::create([node] {node->removeFromParentAndCleanup(true); }),
                nullptr));

    if (--_blood == 0) {
        // 播放音效
        if (_isHost) {
            AudioEngine::play2d("music/player_bomb.mp3");
        } else {
            AudioEngine::play2d("music/enemy-bomb.mp3");
        }
        // 移除该坦克
        exitCamp();
        _detachBullets(); // 让已发射的子弹可以继续有效
        MapLayer::getInstance()->removeAndUnmanageBlock(this);
        printf(">>> player/enemy removed\n");
    } else {
        if (_isHost) {
            // 播放动画
            _isInvincible = true; // 防止掉血
            // 回到出生点, TODO: 等待N秒钟再重生
            setInitialDirection();
            setInitialPosition();
            birth("player1_" + std::to_string((int)_dir) + "_" + std::to_string(_level));
        }
    }
}

void Player::joinCamp(Camp* camp) {
    if (_joinedCamp) {
        exitCamp();
    }
    _joinedCamp = camp;
}

void Player::exitCamp() {
    if (_joinedCamp) {
        _joinedCamp->removePlayer(this);
        _joinedCamp = nullptr;
    }
}

bool Player::hasTeammates() const {
    //return _joinedCamp && _joinedCamp->count() > 1;
    return true; // just for test
}

void Player::onBeCollided(Block* activeBlock) {
    if (activeBlock->getType() == BlockType::BULLET) {
        auto bullet = dynamic_cast<Bullet*>(activeBlock);
        auto bulletCreator = bullet->getCreator();
        if (bulletCreator && (bulletCreator == this || // 自己的子弹
                bulletCreator->getCamp() == this->getCamp())  // 队友的子弹
                ) {
            return; // 忽略自己和队友的子弹
        } else {
            disBlood();
        }
    } else if (activeBlock->getType() == BlockType::PLAYER) {
        auto otherPlayer = dynamic_cast<Player*>(activeBlock);
        if (_isMoving) {
            //auto pos = getPosition();
            //auto pos2 = otherPlayer->getPosition();
            //printf(">>>> onBeCollided by player, self: (%f, %f)\n", pos.x, pos.y);
            //printf(">>>> onBeCollided by player, other: (%f, %f)\n", pos2.x, pos2.y);
            // 如果碰到自己的也是玩家并且是相向碰撞，自己也返回原来的位置
            if (reverseDirection(getDirection()) == otherPlayer->getDirection()) {
                goBack();
            }
        }
    }
}

void Player::onCollidedWith(Vector<Block*>& withBlocks) {
    for (auto block : withBlocks) {
        if (block->getType() == BlockType::WALL ||
                block->getType() == BlockType::STONE ||
                block->getType() == BlockType::PLAYER) {
            // 如果碰到其它障碍物(Wall, Stone, Player)，自己返回原来的位置
            //if (block->getType() == BlockType::PLAYER) {
            //    auto pos = getPosition();
            //    auto pos2 = block->getPosition();
            //    printf("<<<< onCollidedWith by player, self from: (%f, %f)\n", _fromPosition.x, _fromPosition.y);
            //    printf("<<<< onCollidedWith by player, self: (%f, %f)\n", pos.x, pos.y);
            //    printf("<<<< onCollidedWith by player, other: (%f, %f)\n", pos2.x, pos2.y);
            //    printf("<<<< step: %d\n", getMovingStep());
            //}
            goBack();
            break;  // 如果碰到一个障碍物就不能前进
        }
    }
    if (!_isHost) {
        // 敌方坦克碰撞后可以改变方向
        _moveDistance = Player::MAX_MOVE_DISTANCE;
        changeDirection();
    }
}

Vec2 convertToMapPosition(const Vec2& pos) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float x = pos.x + (CENTER_WIDTH - visibleSize.width) / 2;
    float y = pos.y + (CENTER_HEIGHT - visibleSize.height) / 2;
    return Vec2(x, y);
}
