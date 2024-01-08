#pragma once

#include "cocos2d.h"

USING_NS_CC;

class GameStatsPanel;
class GameRuntime;

class GameFinishScene : public Scene {
public:
    static GameFinishScene* create(GameRuntime* gameRuntime);
    bool init(GameRuntime* gameRuntime);

protected:
    void backToMenu();
    void gotoNextStage();

    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);

private:
    GameStatsPanel* _gameStatsPanel = nullptr;
    GameRuntime* _gameRuntime = nullptr;
};
