#pragma once

#include "cocos2d.h"
#include "ui/UILayout.h"

#include "NetworkingMode.h"

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

enum class GameEndpoint {
    NONE,
    LOCAL,
    REMOTE,
};

class GameCard : public Layout {
public:
    static GameCard* createDemo(NetworkingMode nm, GameEndpoint ep);

    static GameCard* create(NetworkingMode nm, GameEndpoint ep,
            Story* story, float width, float height);
    bool init(NetworkingMode nm, GameEndpoint ep,
            Story* story, float width, float height);

    NetworkingMode getNetworkingMode() const { return _networkingMode; }
    GameEndpoint getEndpoint() const { return _endpoint; }
    const Story* getStory() const { return _story; }

protected:
    void obtainGameServerInfo();
    void downloadStoryLine();

private:
    NetworkingMode _networkingMode;
    GameEndpoint _endpoint;
    Story* _story = nullptr;
    GameServer* _gameServer = nullptr;
};
