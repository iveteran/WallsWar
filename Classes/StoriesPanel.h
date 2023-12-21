#pragma once

#include "cocos2d.h"
#include "ui/UILayout.h"

#include "NetworkingMode.h"
#include "SearchBar.h"

USING_NS_CC;
using namespace cocos2d::ui;

class Story;
class CardsView;
class SearchBar;

class StoriesPanel : public Layout {
public:
    static StoriesPanel* create(float width, float height, NetworkingMode nm);
    bool init(float width, float height, NetworkingMode nm);

    void setSearchingCallback(const searchingCallback& cb);
    void setGettingTopNCallback(const gettingTopsCallback& cb, int topN);

    void addStory(Story* story);
    void addStories(const Vector<Story*>& stories);

protected:
    void loadSubscribedStoryCards();
    Vector<Story*> loadStoriesFromPath(const char* path);
    void addBattleCityStoryCard();

private:
    NetworkingMode _networkingMode;

    CardsView* _storyCards = nullptr;
    SearchBar* _searchBar = nullptr;
};
