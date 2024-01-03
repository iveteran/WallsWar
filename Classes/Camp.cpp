#include "RandomUtil.h"
#include "AudioEngine.h"

#include "Camp.h"
#include "Player.h"
#include "MapLayer.h"
#include "User.h"
#include "AI.h"

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
    _ai = CampAI::create(this);
    addChild(_ai);
    _ai->run();
}

Player* Camp::addHost() {
    auto user = User::create();
    _host = addPlayer(user);
    _host->allowCameraFollows();
    return _host;
}

Player* Camp::addPlayer(ActorController* controller) {
    auto player = Player::create(controller);
    addPlayer(player);

    MapLayer::getInstance()->addAndManageBlock(player);

    return player;
}

void Camp::addPlayer(Player* player) {
    player->joinCamp(this);
    _players.insert(player->id(), player);
    _remainPlayers--;
}

void Camp::removePlayer(Player* player) {
    player->exitCamp();
}

void Camp::removePlayer(int playerId) {
    _players.erase(playerId);
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

const Map<int, Camp*>& Camp::getEnemyCamps() const {
    return _enemyCamps;
}

void Camp::addEnemyCamp(Camp* camp) {
    if (camp == nullptr) {
        return;
    }
    _joinedBattle = true;
    _enemyCamps.insert(camp->id(), camp);
    //camp->setEnemyCamp(this);
}

bool Camp::isLost() const {
    return !isCampOk || (_players.empty() && _remainPlayers == 0);
}

bool Camp::isWin() const {
    if (!_joinedBattle) return false;

    for (auto iter : _enemyCamps) {
        auto enemyCamp = iter.second;
        if (!enemyCamp->isLost()) {
            return false;
        }
    }
    return true;
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
