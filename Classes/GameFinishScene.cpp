#include "GameFinishScene.h"
#include "AudioEngine.h"
#include "GameStatsPanel.h"
#include "NetworkingModeMenuScene.h"
#include "GameScene.h"
#include "GameCard.h"

#include "ui/UIImageView.h"

GameFinishScene*
GameFinishScene::create(GameRuntime* gameRuntime) {
    auto *pRet = new(std::nothrow) GameFinishScene();
    if (pRet && pRet->init(gameRuntime)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool GameFinishScene::init(GameRuntime* gameRuntime) {
    if (!Scene::init()) return false;
    _gameRuntime = gameRuntime;

    auto status = _gameRuntime->getStatus();
    const char* bgImage = (status == GameStatus::WIN ?
        "images/game-win-background.png" :
        "images/game-over-background.png");
    auto bg = ImageView::create(bgImage);
    bg->setScale(0.4);
    bg->setPosition(getContentSize() / 2);
    addChild(bg);

    //const char* bgMusic = (status == GameStatus::WIN ? "music/game-win.mp3" : "music/game-fail.mp3");
    const char* bgMusic = (status == GameStatus::WIN ? "music/fail.mp3" : "music/fail.mp3");
    AudioEngine::play2d(bgMusic);

    _gameStatsPanel = GameStatsPanel::create(_gameRuntime);
    _gameStatsPanel->setContentSize(getContentSize() * 0.8);
    _gameStatsPanel->setPosition((getContentSize() - _gameStatsPanel->getContentSize()) / 2);
    _gameStatsPanel->setClickedCallback(CC_CALLBACK_0(GameFinishScene::backToMenu, this));
    _gameStatsPanel->setClickedNextStageCallback(CC_CALLBACK_0(GameFinishScene::gotoNextStage, this));
    addChild(_gameStatsPanel);

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameFinishScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameFinishScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void GameFinishScene::backToMenu() {
    printf(">> game finished, back to menu\n");
    Director::getInstance()->replaceScene(NetworkingModeMenuScene::create());
}

void GameFinishScene::gotoNextStage() {
    printf(">> game current stage finished, goto next stage\n");
    Director::getInstance()->replaceScene(GameScene::create(_gameRuntime));
}

bool GameFinishScene::onTouchBegan(Touch* touch, Event* event) {
    return true;
}

void GameFinishScene::onTouchEnded(Touch* touch, Event* event) {
    auto point = touch->getLocation();
    if (_gameStatsPanel->getBoundingBox().containsPoint(point)) {
        return;
    }

    backToMenu();
}
