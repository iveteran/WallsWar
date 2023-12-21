#pragma once

#include "cocos2d.h"

USING_NS_CC;

namespace cocos2d::ui {
    class LoadingBar;
    class Text;
}
using namespace cocos2d::ui;

class WelcomeScene : public Scene {
public:
    CREATE_FUNC(WelcomeScene);
    virtual bool init() override;

protected:
    bool addBackground();
    void addLoadingBar();
    void addActorAnimations();
    void update(float delta);

    bool onTouch(Touch* touch, Event*);
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void gotoNextScene();

private:
    Sprite* _background = nullptr;
    LoadingBar* _loadingBar = nullptr;
    Text* _elapsedLabel = nullptr;

    float _elapsed = 0.f;
    float _secondTick = 0.f;

    //LoginScene* _loginScene = nullptr;
    //std::string _sessionId;
};
