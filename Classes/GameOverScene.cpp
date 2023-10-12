#include "GameOverScene.h"
#include "AudioEngine.h"
#include "MenuScene.h"

USING_NS_CC;

cocos2d::Scene* GameOverScene::createScene() {
    return GameOverScene::create();
}

bool GameOverScene::init() {
    if (!Scene::init())
        return false;

    auto gameOver = Sprite::create("images/big-gameover.png");
    gameOver->getTexture()->setAliasTexParameters();
    this->addChild(gameOver);
    gameOver->setPosition(this->getContentSize() / 2);

    AudioEngine::play2d("music/fail.mp3");
    scheduleOnce(CC_SCHEDULE_SELECTOR(GameOverScene::_replaceToMenu), 2.0f);

    printf(">> game over init: (%f, %f)\n", gameOver->getPosition().x, gameOver->getPosition().y);
    return true;
}

void GameOverScene::_replaceToMenu(float) {
    printf(">> game over, replace to menu\n");
    Director::getInstance()->replaceScene(MenuScene::create());
}
