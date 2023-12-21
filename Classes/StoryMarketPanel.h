#pragma once

#include "cocos2d.h"

#include "PopupLayer.h"
#include "NetworkingMode.h"

USING_NS_CC;

namespace cocos2d::ui {
    class Text;
    class ListView;
}
using namespace cocos2d::ui;

class Story;
class CardsView;
class SearchBar;

class StoryMarketPanel : public PopupLayer {
public:
    static StoryMarketPanel* create(float width, float height, NetworkingMode nm);
    bool init(float width, float height, NetworkingMode nm);

protected:
    void loadStories();
    void subscribeStory(int storyId);
    void addStoryCard(Story* story);
    void addBattleCityStoryCard();
    void addDemoStoryCard();

    void doSearch(const SearchBar* sb, const char* keywords);
    void getTopN(const SearchBar* sb, int topN);

private:
    NetworkingMode _networkingMode;

    CardsView* _storyCards = nullptr;
};
