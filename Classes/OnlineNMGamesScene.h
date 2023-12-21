#pragma once

#include "cocos2d.h"

USING_NS_CC;
namespace cocos2d::ui {
    class Layout;
}
using namespace cocos2d::ui;

class UISceneTitlePanel;
class CardsView;
class SearchBar;

class OnlineNMGamesScene : public Scene {
public:
    CREATE_FUNC(OnlineNMGamesScene);
    virtual bool init() override;

protected:
    void onMyGamesButtonClicked(const UISceneTitlePanel* titlePanel);
    void doSearch(const SearchBar* sb, const char* keywords);
    void getTopN(const SearchBar* sb, int topN);
    void addDemoGameCard();

private:
    CardsView* _gameCards;
};
