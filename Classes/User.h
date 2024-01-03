#pragma once
#include "ActorController.h"

class Player;

class RemotePeer : public Ref {

private:
    bool _isConnected;
    bool _connectedTime;

    std::string _localIp;
    int _localPort;
    std::string _remoteIp;
    int _remotePort;
};

class User : public ActorController {
public:
    CREATE_FUNC(User);
    virtual bool init() { return ActorController::init(); }

    bool login(const char* username, const char* password) { return false; }
    void logout() {}

    const char* getAvatarFrameName() const { return "player_avatar"; }
    const char* getAvatarImage() const { return "images/player-avatar-yuu.png"; }

    virtual Type getType() const { return ActorController::User; }
    void attachPlayer(Player* player) { _player = player; }

private:
    bool _isLoggedIn = false;
    std::string _session;
    Player* _player = nullptr;

    RemotePeer* _gameClient;
    RemotePeer* _chatClient;
};
