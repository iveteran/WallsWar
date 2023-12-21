#pragma once

#include "cocos2d.h"
#include "NetworkingMode.h"

USING_NS_CC;
using namespace cocos2d::ui;

class UISceneTitlePanel;
class PopupLayer;
class CardsView;
class SearchBar;
class StoryMarketPanel;

class CreatingGameScene : public Scene {
public:
    static CreatingGameScene* create(NetworkingMode nm);
    bool init(NetworkingMode nm);

protected:
    void onGameMarketButtonClicked(const UISceneTitlePanel* titlePanel);

    void loadSubscribedStoryCards();
    void openStoryMarketDialog();
    void subscribeStory();

    void onCloseStoryMarketDailog(PopupLayer* dialog);

    void doSearch(const SearchBar* sb, const char* keywords);
    //void getTopN(const SearchBar* sb, int topN);

private:
    NetworkingMode _networkingMode;

    CardsView* _storyCards = nullptr;
    StoryMarketPanel* _storyMarketDialog = nullptr;
};
