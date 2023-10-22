#include "RandomUtil.h"
#include "AudioEngine.h"

#include "Camp.h"
#include "Player.h"
#include "MapLayer.h"

USING_NS_CC;

void Camp::initSpriteFrameCache() {
    auto camp_0 = Sprite::create("images/block/camp0.png")->getSpriteFrame();
    auto camp_1 = Sprite::create("images/block/camp1.png")->getSpriteFrame();
    camp_0->getTexture()->setAliasTexParameters();
    camp_1->getTexture()->setAliasTexParameters();
    SpriteFrameCache::getInstance()->addSpriteFrame(camp_0, "camp_0");
    SpriteFrameCache::getInstance()->addSpriteFrame(camp_1, "camp_1");
}

bool Camp::init() {
    if (!Campus::init()) {
        return false;
    }

    setFloor(Camp::DefaultFloor);
    initWithSpriteFrameName("camp_0");

    return true;
}

void Camp::enableAI() {
    Player::IsHost = false;
    bool enableAI = true;
    addPlayers(enableAI);
    if (enableAI) {
        enableAutoAddPlayers();
        enableAutoControlPlayers();
    }
}

void Camp::enableAutoAddPlayers(bool enable) {
    if (enable) {
        // 每隔4.5秒添加一名敌人
        schedule(CC_SCHEDULE_SELECTOR(Camp::autoAddPlayers), 5.0f);
    } else {
        unschedule(CC_SCHEDULE_SELECTOR(Camp::autoAddPlayers));
    }
}

void Camp::enableAutoControlPlayers(bool enable) {
    if (enable) {
        schedule(CC_SCHEDULE_SELECTOR(Camp::autoControlPlayersDirection), 0.1f);
        schedule(CC_SCHEDULE_SELECTOR(Camp::autoControlPlayersShoot), 0.5f);
    } else {
        unschedule(CC_SCHEDULE_SELECTOR(Camp::autoControlPlayersDirection));
        unschedule(CC_SCHEDULE_SELECTOR(Camp::autoControlPlayersShoot));
    }
}

void Camp::addPlayers(bool enableAI) {
    // 初始时添加3辆坦克
    if (_remainPlayers == ENEMIES_COUNT) {
        addPlayer(enableAI);
        addPlayer(enableAI);
        addPlayer(enableAI);
    } else {
        if (_remainPlayers == 0) return;

        // 当坦克数量小于6辆时
        if (_players.size() < 6) {
            // 随机添加一辆
            switch (RandomUtil::random(0, 2)) {
            case 0:
                addPlayer(enableAI);
                break;
            case 1:
                addPlayer(enableAI);
                break;
            case 2:
                addPlayer(enableAI);
                break;
            default:
                break;
            }
        }
    }
}

Player* Camp::addPlayer1() {
    Player::IsHost = true;
    auto player1 = addPlayer();
    player1->allowCameraFollows();
    _remainPlayers = 0;
    return player1;
}

Player* Camp::getPlayer1() {
    return _manager;
}

Player* Camp::addPlayer(bool enableAI) {
    auto mapLayer = MapLayer::getInstance();
    auto player = Player::create();
    player->joinCamp(this);
    player->setInitialPosition();
    if (enableAI) {
        player->startMove();
    }
    mapLayer->addAndManageBlock(player);

    if (!_manager) {
        _manager = player;  // set the first player as the camp manager
    }

    _players.insert(player->id(), player);
    _remainPlayers--;

    //auto gameScene = dynamic_cast<GameScene*>(Director::getInstance()->getRunningScene());
    //if (gameScene) {
    //    gameScene->updateInformationArea();
    //}

    return player;
}

void Camp::autoAddPlayers(float) {
    bool enableAI = true;
    addPlayers(enableAI);
}

void Camp::autoControlPlayersDirection(float) {
    for (auto iter : _players) {
        auto player = iter.second;
        player->changeDirection();
    }
}

void Camp::autoControlPlayersShoot(float) {
    for (auto iter : _players) {
        auto player = iter.second;
        // 三分之一的概率发射子弹
        if (RandomUtil::random(1, 3) == 1)
            player->shoot();
    }
}

void Camp::removePlayer(Player* player) {
    _players.erase(player->id());
}

void Camp::onBeCollided(Block* activeBlock) {
    // 忽略本营地的玩家及其子弹的碰撞
    if (activeBlock->getType() == BlockType::PLAYER && dynamic_cast<Player*>(activeBlock)->getCamp()->id() == id() ||
            (activeBlock->getType() == BlockType::BULLET && activeBlock->getCreator()->getCamp()->id() == id())) {
        return;
    }
    AudioEngine::play2d("music/camp_bomb.mp3");
    setTexture(SpriteFrameCache::getInstance()->getSpriteFrameByName("camp_1")->getTexture());
    isCampOk = false;
}

bool Camp::isLost() const {
    return !isCampOk || (_players.empty() && _remainPlayers == 0);
}

bool Camp::isWin() const {
    return _enemyCamp && _enemyCamp->isLost();
}

void Camp::showLostAnimate() {
    // 显示一个从左到右的动画
    auto campLost = Sprite::create("images/gameover.png");
    campLost->getTexture()->setAliasTexParameters();
    MapLayer::getInstance()->addNode(campLost);
    campLost->setPosition(-campLost->getContentSize().width / 2, getPosition().y);
    printf(">> camp lost position: (%f, %f)\n", campLost->getPosition().x, campLost->getPosition().y);

    auto moveTo = MoveTo::create(1.0f, getPosition());
    printf(">> camp lost move to: (%f, %f)\n", getPosition().x, getPosition().y);
    campLost->runAction(Sequence::create(moveTo, 
                DelayTime::create(1.0f),
                CallFunc::create([campLost](){campLost->removeFromParent();}), 
                nullptr));
}
