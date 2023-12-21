#pragma once

#include "cocos2d.h"

USING_NS_CC;
namespace cocos2d::ui {
    class LoadingBar;
    class Text;
    class Button;
    class Layout;
}
using namespace cocos2d::ui;

class CardsView;
class UISceneTitlePanel;

class LocalNMGamesScene : public Scene {
public:
    CREATE_FUNC(LocalNMGamesScene);
    virtual bool init() override;

protected:
    Layout* buildScanningPanel(float width, float height);
    void onMyGamesButtonClicked(const UISceneTitlePanel* titlePanel);
    void addDemoGameCard();

    void update(float delta);

private:
    CardsView* _gameCards = nullptr;

    float _elapsed = 0.f;
    float _secondTick = 0.f;

    LoadingBar* _scanningProgressBar = nullptr;
    Text* _elapsedLabel = nullptr;
    Button* _reScanningBtn = nullptr;
};
