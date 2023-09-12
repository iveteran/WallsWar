#include "GameScene.h"
#include "MapLayer.h"
//#include "Joypad.h"
#include "Joypad2.h"
#include "Common.h"
#include "AudioEngine.h"
#include "PlayerTank.h"
#include "MenuScene.h"
#include "GameOverScene.h"
#include "NumberUtil.h"
#include <vector>

USING_NS_CC;

Scene* GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init())
        return false;

    // 将背景色设置为灰色
    auto background = LayerColor::create(Color4B(100, 100, 100, 200));
    this->addChild(background);

    // 播放开始音乐
    /*AudioEngine::setFinishCallback(AudioEngine::play2d("music/start.mp3"),
        [](int, const std::string &) {
            AudioEngine::play2d("music/bk_sound.mp3", true);
        });*/

        // 展示加载动画
    _showLoadAnimate();

    return true;
}

void GameScene::_showLoadAnimate() {
    auto width = Director::getInstance()->getVisibleSize().width;
    auto mid = Director::getInstance()->getVisibleSize().height / 2;
    auto block1 = LayerColor::create(Color4B(0, 0, 0, 255));
    auto block2 = LayerColor::create(Color4B(0, 0, 0, 255));

    this->addChild(block1);
    this->addChild(block2);

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
        this->removeChild(block1);
    }), nullptr));

    block2->runAction(Sequence::create(action2, CallFunc::create([=]() {
        this->removeChild(block2);
    }), nullptr));


    // 展示Stage
    auto node = Node::create();
    this->addChild(node);
    node->setPosition(this->getContentSize().width / 2 - 10, this->getContentSize().height / 2);

    auto stageSprite = Sprite::create("images/stage.png");
    stageSprite->getTexture()->setAliasTexParameters();
    node->addChild(stageSprite);
    stageSprite->setPosition(0, 0);

    auto tenSprite = NumberUtil::getBlackNumber(stage / 10);
    tenSprite->getTexture()->setAliasTexParameters();
    node->addChild(tenSprite);
    tenSprite->setPosition(stageSprite->getPositionX() + stageSprite->getContentSize().width, stageSprite->getPositionY());

    if (stage / 10 == 0) tenSprite->setVisible(false);

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
        this->_initMapLayer();
        //this->_addJoypad();
        this->_addJoypad2();
        this->schedule(CC_SCHEDULE_SELECTOR(GameScene::_checkGameStatus), 0.2f);
    }),
        nullptr)
    );
}

void GameScene::_initMapLayer() {
    _map = MapLayer::getInstance();
    this->addChild(_map);

    // 设置地图位置
    _map->setContentSize(Size(CENTER_WIDTH, CENTER_HEIGHT));
    _map->setIgnoreAnchorPointForPosition(false);
    _map->setPosition(Director::getInstance()->getVisibleSize() / 2);

    // 加载地图数据
    _map->loadLevelData(stage);

    // 更新信息
    updateInformationArea(true);

    // 添加玩家和敌人
    _player = _map->addPlayer();
    _map->addEnemies();

    // 自动控制敌人
    _map->enableAutoAddEnemies();
    _map->enableAutoControlEnemies();
}

//void GameScene::_addJoypad() {
//    _joypad = Joypad::getInstance();
//    _joypad->setPlayer(_player);
//    this->addChild(_joypad);
//}

void GameScene::_addJoypad2() {
    _joypad2 = Joypad2::create();
    _joypad2->setJoystickType(JoystickType::KEY4);
    _joypad2->setPlayer(_player);
    this->addChild(_joypad2);
}

void GameScene::_checkGameStatus(float) {
    // 停止所有音乐
    AudioEngine::stopAll();
    if (_map->getPlayers().size() == 0 || !_map->isCampOk) {
        // 进入失败场景
        this->unscheduleAllCallbacks();
        _eventDispatcher->removeAllEventListeners();

        scheduleOnce(CC_SCHEDULE_SELECTOR(GameScene::_gameover), 2.0f);
    } else if (_map->remainTank == 0 && _map->getEnemies().size() == 0) {
        // 进入结算场景
        this->cleanup();
        this->removeAllChildrenWithCleanup(true);

        auto scene = GameScene::create((this->stage + 1) % (STAGE_COUNT + 1));
        Director::getInstance()->replaceScene(scene);
    }
}

void GameScene::_gameover(float) {
    auto gameover = Sprite::create("images/gameover.png");
    gameover->getTexture()->setAliasTexParameters();
    this->addChild(gameover);
    gameover->setPosition({ this->getContentSize().width / 2, -gameover->getContentSize().height / 2 });
    auto moveTo = MoveTo::create(2.0f, { this->getContentSize().width / 2, this->getContentSize().height / 2 });

    gameover->runAction(Sequence::create(
        moveTo,
        DelayTime::create(1.0f),
        CallFunc::create([this] {
        this->cleanup();
        this->removeAllChildrenWithCleanup(true);

        Director::getInstance()->replaceScene(GameOverScene::createScene());
    }),
        nullptr
        ));


}

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

        for (int i = 0; i != _map->remainTank; i++) {
            auto icon = Sprite::createWithSpriteFrame(enemyIcon);
            icon->getTexture()->setAliasTexParameters();
            this->addChild(icon);
            icon->setPosition(x + (i % 2) * icon->getContentSize().width,
                              y - (i / 2) * icon->getContentSize().height);
            sprites.push_back(icon);
        }
    } else {
        sprites.back()->removeFromParent();
        sprites.pop_back();
    }

}

void GameScene::addSpriteFrameCache() {
    auto spriteFrameCache = SpriteFrameCache::getInstance();

    spriteFrameCache->addSpriteFrame(Sprite::create("images/enemytank-ico.png")->getSpriteFrame(), "enemy_icon");
    spriteFrameCache->addSpriteFrame(Sprite::create("images/1P.png")->getSpriteFrame(), "1p");
    spriteFrameCache->addSpriteFrame(Sprite::create("images/2P.png")->getSpriteFrame(), "2p");
    spriteFrameCache->addSpriteFrame(Sprite::create("images/playertank-ico.png")->getSpriteFrame(), "player_icon");
    spriteFrameCache->addSpriteFrame(Sprite::create("images/flag.png")->getSpriteFrame(), "flag");

}
