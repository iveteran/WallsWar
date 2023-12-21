#pragma once

#include "cocos2d.h"
#include "PlayerGamesScene.h"
#include "NetworkingMode.h"

USING_NS_CC;

class SingleNMGamesScene : public PlayerGamesScene {
public:
    CREATE_FUNC(SingleNMGamesScene);
    virtual bool init() override {
        return PlayerGamesScene::init(NetworkingMode::SINGLE);
    }
};
