#pragma once

#include "cocos2d.h"
#include "ui/UILayout.h"

USING_NS_CC;
using namespace cocos2d::ui;

class Story;

class GameServer : public Ref {
public:
    CREATE_FUNC(GameServer);
    bool init();

private:
    std::string _name;
    std::string _ip;
    std::string _owner;
    std::string _description;
};

class GameCard : public Layout {
public:
    static GameCard* createDemo();

    static GameCard* create(Story* story, float width, float height);
    bool init(Story* story, float width, float height);

protected:
    void obtainGameServerInfo();
    void downloadStoryLine();

private:
    Story* _story = nullptr;
    GameServer* _gameServer = nullptr;
};
