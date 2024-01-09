#include <vector>

#include "AudioEngine.h"
#include "MenuScene.h"
#include "GameFinishScene.h"
#include "NumberUtil.h"

#include "GameScene.h"
#include "MapLayer.h"
#include "Camp.h"
#include "ControlLayer.h"
#include "GameCard.h"
#include "constants/SceneConstants.h"

USING_NS_CC;

GameScene* GameScene::create(GameRuntime* gameRuntime) {
    CCLOG("-------- GameScene::create -----------");

    auto* pRet = new(std::nothrow) GameScene();
    if (pRet) {
        //pRet->stage = stage;
        pRet->_gameRuntime = gameRuntime;
        if (pRet->init()) {
            pRet->autorelease();
            return pRet;
        }
    }
    delete pRet;

    return nullptr;
}

GameScene::~GameScene() {
    CCLOG("-------- GameScene::~GameScene -----------");
}

void GameScene::onEnter()
{
    CCLOG("GameScene onEnter");
    Scene::onEnter();
}

void GameScene::onExit()
{
    Scene::onExit();
    CCLOG("GameScene onExit");
}

bool GameScene::init() {
    if (!Scene::init())
        return false;

    initSpriteFrameCache();

    // 播放开始音乐
    /*AudioEngine::setFinishCallback(AudioEngine::play2d("music/start.mp3"),
        [](int, const std::string &) {
            AudioEngine::play2d("music/bk_sound.mp3", true);
        });*/

    // 展示加载动画
    //_showLoadAnimate();

    _initMapLayer();
    _initControlLayer();

    CCLOG("-------- GameScene::init -----------");
    _printGameInfo();
    CCLOG("------------------------------------");

    // 默认渲染更新
    scheduleUpdate();

    return true;
}

void GameScene::_printGameInfo()
{
    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    Size visible_size = Director::getInstance()->getVisibleSize();
    CCLOG(">> visible_origin: (%f, %f)", visible_origin.x, visible_origin.y);
    CCLOG(">> visible_size: (%f, %f)", visible_size.width, visible_size.height);

    Size win_size = Director::getInstance()->getWinSize();
    CCLOG(">> win_size: (%f, %f)", win_size.width, win_size.height);

    auto camera = Camera::getDefaultCamera();
    CCLOG(">> camera type: %d", camera->getType());
    auto cameraPos = camera->getPosition();
    CCLOG(">> camera pos: (%f, %f)", cameraPos.x, cameraPos.y);
    auto vp = camera->getDefaultViewport();
    CCLOG(">> camera viewport: (%f, %f, %f, %f)", vp.x, vp.y, vp.w, vp.h);
}

#if 0
void GameScene::_showLoadAnimate() {
    auto width = Director::getInstance()->getVisibleSize().width;
    auto mid = Director::getInstance()->getVisibleSize().height / 2;
    auto block1 = LayerColor::create(Color4B(0, 0, 0, 255));
    auto block2 = LayerColor::create(Color4B(0, 0, 0, 255));

    addChild(block1);
    addChild(block2);

    //从上往下
    block1->setContentSize(Size(width, mid));
    block1->setPosition(Vec2(0, mid * 2));

    //从下往上
    block2->setContentSize(Size(width, mid));
    block2->setPosition(Vec2(0, -mid));

    auto delayTime = 0.5f;

    auto action1 = MoveBy::create(delayTime, Vec2(0, -mid));
    auto action2 = MoveBy::create(delayTime, Vec2(0, mid));

    block1->runAction(Sequence::create(action1, CallFunc::create([=]() {
        removeChild(block1);
    }), nullptr));

    block2->runAction(Sequence::create(action2, CallFunc::create([=]() {
        removeChild(block2);
    }), nullptr));


    // 展示Stage
    auto node = Node::create();
    addChild(node);
    node->setPosition(getContentSize().width / 2 - 10, getContentSize().height / 2);

    auto stageSprite = Sprite::create("images/stage.png");
    stageSprite->getTexture()->setAliasTexParameters();
    node->addChild(stageSprite);
    stageSprite->setPosition(0, 0);

    auto tenSprite = NumberUtil::getBlackNumber(stage / 10);
    tenSprite->getTexture()->setAliasTexParameters();
    node->addChild(tenSprite);
    tenSprite->setPosition(stageSprite->getPositionX() + stageSprite->getContentSize().width, stageSprite->getPositionY());

    if (stage / 10 == 0)
        tenSprite->setVisible(false);

    auto sigSprite = NumberUtil::getBlackNumber(stage % 10);
    sigSprite->getTexture()->setAliasTexParameters();
    node->addChild(sigSprite);
    sigSprite->setPosition(tenSprite->getPositionX() + tenSprite->getContentSize().width, stageSprite->getPositionY());

    node->runAction(Sequence::create(
        DelayTime::create(delayTime),
        Show::create(),
        DelayTime::create(1),
        CallFunc::create([this, node]() {
            node->removeFromParentAndCleanup(true);
            _initMapLayer();
            _initControlLayer();
            //schedule(CC_SCHEDULE_SELECTOR(GameScene::_checkGameStatus), 0.2f);
            }),
        nullptr)
    );
}
#endif

void GameScene::_initMapLayer() {
    _map = MapLayer::getInstance();
    addChild(_map);

    // 加载地图数据
    //_map->loadLevelData(stage);
    _map->loadGame(_gameRuntime);
    _player1 = _map->getPlayer1();
    if (_ctrlLayer) {
        _ctrlLayer->attachPlayer(_player1);
    }

    // 更新信息
    //updateInformationArea(true);
}

void GameScene::_initControlLayer() {
    _ctrlLayer = ControlLayer::create();
    if (_player1) {
        _ctrlLayer->attachPlayer(_player1);
    }
    addChild(_ctrlLayer);
}

void GameScene::update(float dt) {
    //CCLOG("GameScene::update dt: %f", dt);
    _checkGameStatus(dt);
}

void GameScene::_checkGameStatus(float dt) {
    //CCLOG("GameScene::_checkGameStatus dt: %f", dt);
    // 停止所有音乐
    AudioEngine::stopAll();
    if (!_map || !_map->getHostCamp()) {
        return;
    }
    if (_map->getHostCamp()->isLost()) {
        // 进入失败场景
        _map->getHostCamp()->showLostAnimate();

        unscheduleAllCallbacks();
        _eventDispatcher->removeAllEventListeners();

        _gameRuntime->setStatus(GameStatus::FAILURE);
        scheduleOnce(CC_SCHEDULE_SELECTOR(GameScene::_gameover), 2.0f);
    } else if (_map->getHostCamp()->isWin()) {
        // 进入结算场景
        _gameRuntime->setStatus(GameStatus::WIN);
        scheduleOnce(CC_SCHEDULE_SELECTOR(GameScene::_gamewin), 2.0f);
    }
}

void GameScene::_gameover(float) {
    auto gameover = Sprite::create("images/gameover.png");
    gameover->getTexture()->setAliasTexParameters();
    addChild(gameover);
    gameover->setPosition({ getContentSize().width / 2, -gameover->getContentSize().height / 2 });
    auto moveTo = MoveTo::create(2.0f, { getContentSize().width / 2, getContentSize().height / 2 });

    gameover->runAction(Sequence::create(
        moveTo,
        DelayTime::create(1.0f),
        CallFunc::create([this] {
            cleanup();
            removeAllChildrenWithCleanup(true);

            Director::getInstance()->replaceScene(GameFinishScene::create(_gameRuntime));
        }),
        nullptr
        ));
}

void GameScene::_gamewin(float) {
    cleanup();
    removeAllChildrenWithCleanup(true);

    Director::getInstance()->replaceScene(GameFinishScene::create(_gameRuntime));
}

/*
void GameScene::updateInformationArea(bool first) {
    static std::vector<Sprite*> sprites;
    if (first) {
        sprites.clear();
        auto spriteFrameCache = SpriteFrameCache::getInstance();
        // 绘制剩余坦克图标
        // 左上角坐标
        auto x = _map->getPositionX() + _map->getContentSize().width / 2 + 7;
        auto y = _map->getPositionY() + _map->getContentSize().height / 2 - 10;
        auto enemyIcon = spriteFrameCache->getSpriteFrameByName("enemy_icon");

        for (int i = 0; i != _map->getEnemyCamp()->getRemainPlayers(); i++) {
            auto icon = Sprite::createWithSpriteFrame(enemyIcon);
            icon->getTexture()->setAliasTexParameters();
            addChild(icon);
            icon->setPosition(x + (i % 2) * icon->getContentSize().width,
                              y - (i / 2) * icon->getContentSize().height);
            sprites.push_back(icon);
        }
    } else {
        if (sprites.size() > 0) {
            sprites.back()->removeFromParent();
            sprites.pop_back();
        }
    }

}
*/

void GameScene::initSpriteFrameCache() {
    /*
    auto spriteFrameCache = SpriteFrameCache::getInstance();

    spriteFrameCache->addSpriteFrame(Sprite::create("images/enemytank-ico.png")->getSpriteFrame(), "enemy_icon");
    spriteFrameCache->addSpriteFrame(Sprite::create("images/1P.png")->getSpriteFrame(), "1p");
    spriteFrameCache->addSpriteFrame(Sprite::create("images/2P.png")->getSpriteFrame(), "2p");
    spriteFrameCache->addSpriteFrame(Sprite::create("images/playertank-ico.png")->getSpriteFrame(), "player_icon");
    spriteFrameCache->addSpriteFrame(Sprite::create("images/flag.png")->getSpriteFrame(), "flag");
    */
    MapLayer::initSpriteFrameCache();
}
