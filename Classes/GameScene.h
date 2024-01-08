#pragma once

#include "cocos2d.h"

class Player;
class ControlLayer;
class MapLayer;
class GameRuntime;
class Camp;

class GameScene : public cocos2d::Scene {
public:
    static GameScene* create(GameRuntime* gameRuntime);

    ~GameScene();
    bool init() override;

    virtual void onEnter();
    virtual void onExit();
    virtual void update(float dt);

    static void initSpriteFrameCache();
    //void updateInformationArea(bool first = false);  // 更新右侧信息区域

    Player* getPlayer1() const { return _player1; }
    ControlLayer* getControlLayer() const { return _ctrlLayer; }
    MapLayer* getMapLayer() const { return _map; }
    GameRuntime* getGameRuntime() { return _gameRuntime; }

    //short stage = 1;                                          // 当前关卡

private:
    ControlLayer* _ctrlLayer = nullptr;                      // 游戏控制、信息和状态显示
    MapLayer* _map = nullptr;                                // 管理地图
    Player* _player1 = nullptr;
    GameRuntime* _gameRuntime = nullptr;

    void _printGameInfo();
    //void _showLoadAnimate();                                 // 展示载入关卡动画
    void _initMapLayer();                                    // 初始化地图数据
    void _initControlLayer();
    void _checkGameStatus(float);                            // 检查游戏状态
    void _gameover(float);                                   // 游戏结束动画
    void _gamewin(float);
};

inline Player* GET_PLAYER1() {
    auto gameScene = dynamic_cast<GameScene*>(cocos2d::Director::getInstance()->getRunningScene());
    return gameScene != nullptr ? gameScene->getPlayer1() : nullptr;
}

inline ControlLayer* GET_CONTROL_LAYER() {
    auto gameScene = dynamic_cast<GameScene*>(cocos2d::Director::getInstance()->getRunningScene());
    return gameScene != nullptr ? gameScene->getControlLayer() : nullptr;
}

inline MapLayer* GET_MAP_LAYER() {
    auto gameScene = dynamic_cast<GameScene*>(cocos2d::Director::getInstance()->getRunningScene());
    return gameScene != nullptr ? gameScene->getMapLayer() : nullptr;
}
