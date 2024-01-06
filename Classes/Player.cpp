#include "Player.h"
#include "MapLayer.h"
#include "ControlLayer.h"
#include "GameScene.h"
#include "Bullet.h"
#include "Camp.h"
#include "constants/CampConstants.h"
#include "AI.h"
#include "User.h"

#include "RandomUtil.h"
#include "AudioEngine.h"

USING_NS_CC;

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
static const float defaultSpriteScale = 0.25f;

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

    loadFrameAnimation();
}

void Player::loadFrameAnimation() {

    auto sf1 = Sprite::create("images/player-avatar-anonymous.png")->getSpriteFrame();
    sf1->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(sf1, "anonymous_avatar");

    auto sf2 = Sprite::create("images/player-avatar-yuu.png")->getSpriteFrame();
    sf2->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(sf2, "player_avatar");

    auto sf3 = Sprite::create("images/player-blood-ring-3-3.png")->getSpriteFrame();
    sf3->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(sf3, "player-blood-ring-3-3");

    auto sf3_1 = Sprite::create("images/player-blood-ring-3-1.png")->getSpriteFrame();
    sf3_1->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(sf3_1, "player-blood-ring-3-1");

    auto sf3_2 = Sprite::create("images/player-blood-ring-3-2.png")->getSpriteFrame();
    sf3_2->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(sf3_2, "player-blood-ring-3-2");

    auto sf4 = Sprite::create("images/player-direction-indicator.png")->getSpriteFrame();
    sf4->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(sf4, "player-direction-indicator");
}

Player* Player::create(Camp* camp, ActorController* controller) {
    auto pRet = new(std::nothrow) Player();
    if (pRet && pRet->init(camp, controller)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool Player::init(Camp* camp, ActorController* controller) {
    if (!Actor::init()) {
        return false;
    }

    setScale(defaultSpriteScale);
    //_canMove = false;

    if (!camp) {
        camp = MapLayer::getInstance()->getPublicCamp();
    }
    joinCamp(camp);

    const char* avatarFrameName = "anonymous_avatar";
    if (controller) {
        _controller = controller;
        _isHost = _controller->getType() == ActorController::User;
        auto user = dynamic_cast<User*>(_controller);
        avatarFrameName = user->getAvatarFrameName();
        _level = 2;
    } else {
        _ai = PlayerAI::create(this);
        _ai->run();
        addChild(_ai);
    }

    initWithSpriteFrameName(avatarFrameName);

    addBloodRing();
    addDirectionIndicator();
    //addShadow();

    _initBullets();

    birth();

    return true;
}

const char* Player::getAvatarImage() const {
    if (_isHost) {
        auto user = dynamic_cast<User*>(_controller);
        return user->getAvatarImage();
    } else {
        return "images/game-manager.png";
    }
}

void Player::addBloodRing() {
    _bloodRing = Sprite::create();
    auto spriteFrameName = StringUtils::format("player-blood-ring-3-%d", _blood);
    _bloodRing->initWithSpriteFrameName(spriteFrameName);
    _bloodRing->setPosition(getContentSize() / 2);
    addChild(_bloodRing);

}

void Player::addDirectionIndicator() {
    _dirIndicator = Sprite::create();
    _dirIndicator->initWithSpriteFrameName("player-direction-indicator");
    _dirIndicator->setPosition(getContentSize() / 2);
    addChild(_dirIndicator);
}

Direction Player::getInitialDirection() const {
    /*
    Direction dir = Direction::NONE;
    auto campPos = _joinedCamp->getPosition();
    //auto enemyCampPos = _enemyCamp->getPosition();
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
        dir = Direction::UP;
        int xDistance = abs(campPos.x - enemyCampPos.x);
        if (campPos.y < enemyCampPos.y) {
            int yDistance = abs(campPos.y - enemyCampPos.y);
            if (yDistance < xDistance) {
                dir = Direction::UP;
            }
        }
    }
    return dir;
    */
    return Direction::UP;
}

void Player::setInitialDirection() {
    setDirection(getInitialDirection());
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
            //bullet->destroy();
        }
    }
}

void Player::playAnimate() {
}

void Player::stopAnimate() {
    if (!_canMove) {
        return;
    }
    stopAllActions();
}

void Player::playFallingAnimate() {
    // rising, falling
}

int Player::getMovingStep() const {
    int step = MovableBlock::getMovingStep() + _level * 0.2f;
    return step;
}

void Player::birth() {
    //_canMove = false;
    _blood = 3;

    stopAllActions();

    auto spriteFrameName = StringUtils::format("player-blood-ring-3-%d", _blood);
    _bloodRing->initWithSpriteFrameName(spriteFrameName);
    addToBattlefield();
    setInitialDirection();
    setInitialPosition();

    //Vector<SpriteFrame*> spriteFrames;
    //auto animate = createBirthEffectAnimate(spriteFrames);

    runAction(Sequence::create(
        //animate,
        CallFunc::create([=]() {
            beInvincible(3);
            //_canMove = true;
            if (_isHost) {
                moveCameraToPlayer();
            }
        }),
        nullptr
    ));
}

void Player::birthScheduler(float dt) {
    birth();
}

Animate* Player::createBirthEffectAnimate(Vector<SpriteFrame*>& spriteFrames) {
    auto spriteFrameCache = SpriteFrameCache::getInstance();
    for (int i = 0; i < 4 * 2; i++) {
        std::string n = std::to_string(i % 4);
        auto spriteFrame = spriteFrameCache->getSpriteFrameByName("star_" + n);
        spriteFrames.pushBack(spriteFrame);
    }

    // 星星动画
    auto animation = Animation::createWithSpriteFrames(spriteFrames, 0.2f);
    auto animate = Animate::create(animation);

    return animate;
}

void Player::moveCameraToPlayer() {
    auto camera = Camera::getDefaultCamera();
    auto originCameraPos = camera->getPosition();
    printf(">> [moveCameraToPlayer] camera pos: (%f, %f)\n", originCameraPos.x, originCameraPos.y);
    auto cameraPos = convertToMapPosition(originCameraPos);
    printf(">> [moveCameraToPlayer] camera pos for map: (%f, %f)\n", cameraPos.x, cameraPos.y);

    auto playerPos = getPosition();
    printf(">> [moveCameraToPlayer] player pos: (%f, %f)\n", playerPos.x, playerPos.y);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    float xOffset = 0, yOffset = 0;
    if (cameraPos.x > playerPos.x) {
        xOffset = cameraPos.x - playerPos.x - visibleSize.width / 2 + Camp::SIZE  / 2;
        xOffset += std::min(playerPos.x - Camp::SIZE / 2, movingMinBoundaryGap);
        xOffset = xOffset > 0 ? xOffset * -1 : 0;
    } else {
        xOffset = playerPos.x - cameraPos.x - visibleSize.width / 2 + Camp::SIZE  / 2;
        xOffset += std::min(CENTER_WIDTH - playerPos.x - Camp::SIZE / 2, movingMinBoundaryGap);
        xOffset = xOffset > 0 ? xOffset : 0;
    }
    if (cameraPos.y > playerPos.y) {
        yOffset = cameraPos.y - playerPos.y - visibleSize.height / 2 + Camp::SIZE / 2;
        yOffset += std::min(playerPos.y - Camp::SIZE / 2, movingMinBoundaryGap);
        yOffset = yOffset > 0 ? yOffset * -1 : 0;
    } else {
        yOffset = playerPos.y - cameraPos.y - visibleSize.height / 2 + Camp::SIZE / 2;
        yOffset += std::min(CENTER_HEIGHT - playerPos.y - Camp::SIZE / 2, movingMinBoundaryGap);
        yOffset = yOffset > 0 ? yOffset : 0;
    }

    printf(">> [moveCameraToPlayer] offset: (%f, %f)\n", xOffset, yOffset);
    if (xOffset != 0 || yOffset != 0 ) {
        // make moving effect
        float movingTime = std::max(abs(xOffset), abs(yOffset)) * 0.01f;
        movingTime = std::min(movingTime, 3.0f);
        printf(">> [moveCameraToPlayer] movingTime: %f\n", movingTime);

        camera->runAction(MoveBy::create(movingTime, Vec2(xOffset, yOffset)));

        auto ctrlLayer = GET_CONTROL_LAYER();
        if (ctrlLayer != nullptr) {
            ctrlLayer->runAction(MoveBy::create(movingTime, Vec2(xOffset, yOffset)));
        }

        CCLOG(">> [moveCameraToPlayer] camera move to position: (%f, %f)",
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

bool Player::setDirection(Direction dir) {
    bool changed = MovableBlock::setDirection(dir);
    if (changed && _dirIndicator) {
        float degree = calculateRotateDegree(_fromDir, _dir);
        float durationTime = 0.2f;
        stopAnimate();
        _dirIndicator->runAction(RotateBy::create(durationTime, degree));
        playAnimate();
    }
    return changed;
}

void Player::shoot() {
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
    _blood--;

    if (_blood > 0) {
        showAttackedEffect();
        changeBloodRing();
    } else {
        _detachBullets(); // 让已发射的子弹可以继续有效

        if (_joinedCamp->requestLifeOk()) {
            showDeadEffect();
            returnToCamp();
            // 回到出生点, TODO: show waiting message at window top
            scheduleOnce(CC_SCHEDULE_SELECTOR(Player::birthScheduler), 2.0f);
            printf(">>> player/enemy dead, wait 2 seconds to rebirth\n");
        } else {
            if (_ai) {
                _ai->stop();
            }
            exitCamp(); // 移除该坦克
            //MapLayer::getInstance()->removeAndUnmanageBlock(this);
            removeFromBattlefield();
            showKickoutEffect();
            printf(">>> player/enemy dead, have no more life, kickout\n");
        }
    }
}

void Player::returnToCamp() {
    setVisible(false);
    setPosition(_joinedCamp->getPosition());
}

void Player::removeFromBattlefield() {
    setVisible(false);
    MapLayer::getInstance()->unmanageBlock(this);
}

void Player::addToBattlefield() {
    setVisible(true);
    MapLayer::getInstance()->manageBlock(this);
}

void Player::showAttackedEffect() {
    // TODO: freeze the player one second
    showDeadEffect();
}

void Player::changeBloodRing() {
    printf(">>> player/enemy disblood, replace blood ring\n");
    auto spriteFrameName = StringUtils::format("player-blood-ring-3-%d", _blood);
    _bloodRing->initWithSpriteFrameName(spriteFrameName);
}

void Player::showDeadEffect() {
    // 播放音效
    if (_isHost) {
        AudioEngine::play2d("music/player_bomb.mp3");
    } else {
        AudioEngine::play2d("music/enemy-bomb.mp3");
    }

    auto spriteFrameCache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> spriteFrames;

    for (int i = 0; i != 5; i++) {
        std::string n = std::to_string(i);
        auto spriteFrame = spriteFrameCache->getSpriteFrameByName("blast_" + n);
        spriteFrames.pushBack(spriteFrame);
    }

    // TODO 每次死亡都重新构造动画
    auto blastAnimation = Animation::createWithSpriteFrames(spriteFrames, 0.1f);
    auto blastAnimate = Animate::create(blastAnimation);

    // 播放动画
    auto mapLayer = MapLayer::getInstance();
    auto node = Sprite::create();
    mapLayer->addNode(node);
    node->setPosition(getPosition());
    node->runAction(Sequence::create(blastAnimate,
                CallFunc::create([node] {node->removeFromParentAndCleanup(true); }),
                nullptr));
}

void Player::showKickoutEffect() {
}

void Player::handleBeAttacked(const Weapon* weapon) {
    if (_isInvincible)
        return;

    disBlood();
    auto enemy = dynamic_cast<const Player*>(weapon->getCreator());
    if (!enemy) {
        return;
    }
    addEnemy(enemy);
    auto enemyCamp = enemy->getCamp();
    if (enemyCamp && _joinedCamp) {
        _joinedCamp->addEnemyCamp(enemyCamp);
    }
}

void Player::addEnemy(const Player* enemy) {
    _enemies.insert(enemy->id(), (Player*)enemy);
}

void Player::removeEnemy(int playerId) {
    _enemies.erase(playerId);
}

bool Player::isEnemy(int playerId) {
    return _enemies.find(playerId) != _enemies.end();
}

void Player::joinCamp(Camp* camp) {
    if (_joinedCamp) {
        exitCamp();
    }
    _joinedCamp = camp;

    //setInitialPosition();
}

void Player::exitCamp() {
    if (_joinedCamp) {
        _joinedCamp->removePlayer(id());
        _joinedCamp = MapLayer::getInstance()->getPublicCamp();
    }
}

void Player::addEnemyCamp(Camp* camp) {
    _joinedCamp->addEnemyCamp(camp);
    //setInitialDirection();
}

const Map<int, Camp*>& Player::getEnemyCamps() const {
    static const Map<int, Camp*> _emptyCamps;
    return _joinedCamp ? _joinedCamp->getEnemyCamps() : _emptyCamps;
}

bool Player::isFreeMan() const {
    return !_joinedCamp;
}

bool Player::hasTeammates() const {
    //return _joinedCamp && _joinedCamp->count() > 1;
    return true; // FIXME: just for test
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
            handleBeAttacked(bullet);
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
    if (_ai) {
        _ai->handleCollidedWith(withBlocks);
    }
}

Vec2 convertToMapPosition(const Vec2& pos) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float x = pos.x + (CENTER_WIDTH - visibleSize.width) / 2;
    float y = pos.y + (CENTER_HEIGHT - visibleSize.height) / 2;
    return Vec2(x, y);
}
