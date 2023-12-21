#pragma once

#include "cocos2d.h"
#include "NetworkingMode.h"

USING_NS_CC;
namespace cocos2d::ui {
    class Layout;
}
using namespace cocos2d::ui;

class UISceneTitlePanel;
class CardsView;
class SearchBar;

class PlayerGamesScene: public Scene {
public:
    static PlayerGamesScene* create(NetworkingMode nmMode);
    bool init(NetworkingMode nmMode);

protected:
    void onCreatingButtonClicked(const UISceneTitlePanel* titlePanel);
    void doSearch(const SearchBar* sb, const char* keywords);
    void addDemoGameCard();
    void addBattleCityGameCard();

private:
    NetworkingMode _networkingMode;
    CardsView* _gameCards;
};
