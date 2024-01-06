#include "AI.h"
#include "Camp.h"
#include "Player.h"

#include "RandomUtil.h"

PlayerAI* PlayerAI::create(Player* player) {
    auto pRet = new(std::nothrow) PlayerAI();
    if (pRet && pRet->init(player)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool PlayerAI::init(Player* player) {
    if (!AI::init()) return false;
    _player = player;

    //scheduleUpdate();
    return true;
}

void PlayerAI::run() {
    _player->startMove();
    enableAutoControl();
}

void PlayerAI::stop() {
    enableAutoControl(false);
}

void PlayerAI::update(float dt) {
}

void PlayerAI::enableAutoControl(bool enable) {
    if (enable) {
        schedule(CC_SCHEDULE_SELECTOR(PlayerAI::autoControlDirection), 5.0f);
        schedule(CC_SCHEDULE_SELECTOR(PlayerAI::autoControlShoot), 0.5f);
    } else {
        unschedule(CC_SCHEDULE_SELECTOR(PlayerAI::autoControlDirection));
        unschedule(CC_SCHEDULE_SELECTOR(PlayerAI::autoControlShoot));
    }
}

void PlayerAI::autoControlDirection(float dt) {
    changeDirectionRandomly();
}

void PlayerAI::changeDirectionRandomly() {
    auto select = RandomUtil::random(1, 10);
    if (select <= 4) {
        _player->setDirection(Direction::DOWN);
    } else if (select <= 6) {
        _player->setDirection(Direction::UP);
    } else if (select <= 8) {
        _player->setDirection(Direction::LEFT);
    } else {
        _player->setDirection(Direction::RIGHT);
    }
}

void PlayerAI::handleCollidedWith(Vector<Block*>& withBlocks) {
    changeDirectionRandomly();
}

void PlayerAI::autoControlShoot(float) {
    // 三分之一的概率发射子弹
    if (RandomUtil::random(1, 3) == 1)
        _player->shoot();
}

CampAI* CampAI::create(Camp* camp) {
    auto pRet = new(std::nothrow) CampAI();
    if (pRet && pRet->init(camp)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool CampAI::init(Camp* camp) {
    if (!AI::init()) return false;
    _camp = camp;

    _camp->addPlayer();
    _camp->addPlayer();
    _camp->addPlayer();

    //scheduleUpdate();
    return true;
}

void CampAI::run() {
    enableAutoAddPlayers();
}

void CampAI::stop() {
    enableAutoAddPlayers(false);
}

void CampAI::update(float dt) {
}

void CampAI::enableAutoAddPlayers(bool enable) {
    if (enable) {
        schedule(CC_SCHEDULE_SELECTOR(CampAI::autoAddPlayers), 5.0f);
    } else {
        unschedule(CC_SCHEDULE_SELECTOR(CampAI::autoAddPlayers));
    }
}

void CampAI::autoAddPlayers(float) {
    addPlayers();
}

void CampAI::addPlayers() {
    if (_camp->getRemainPlayers() == 0) return;

    // 随机添加一到三个
    int count = RandomUtil::random(0, 2);
    for (int i=0; i<count; i++) {
        _camp->addPlayer();
    }
}
