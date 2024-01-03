#pragma once

#include "cocos2d.h"

using cocos2d::Node;

class Actor;

class ActorController : public Node {
public:
    enum Type {
        User,
        PlayerAgent,
        AI,
    };

    //CREATE_FUNC(ActorController);
    virtual bool init() { return Node::init(); }

    virtual Type getType() const = 0;

    void attachActor(Actor* actor) { _actor = actor; }

private:
    Actor* _actor = nullptr;
};

class PlayerAgent : ActorController {
public:
    virtual Type getType() const { return ActorController::PlayerAgent; }
};
