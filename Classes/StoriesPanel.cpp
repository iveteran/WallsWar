#include "StoriesPanel.h"
#include "StoryCard.h"
#include "CardsView.h"
#include "SearchBar.h"

StoriesPanel* StoriesPanel::create(float width, float height, NetworkingMode nm) {
    auto pRet = new(std::nothrow) StoriesPanel();
    if (pRet && pRet->init(width, height, nm)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool StoriesPanel::init(float width, float height, NetworkingMode nm) {
    if (!Layout::init()) {
        return false;
    }
    _networkingMode = nm;

    setContentSize(Size(width, height));
    setLayoutType(Layout::Type::VERTICAL);

    auto lpV = LinearLayoutParameter::create();
    lpV->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    _searchBar = SearchBar::create();
    _searchBar->setLayoutParameter(lpV->clone());
    addChild(_searchBar);

    _storyCards = CardsView::create(width, height * 0.8);
    _storyCards->setLayoutParameter(lpV->clone());
    addChild(_storyCards);

    addBattleCityStoryCard();
    loadSubscribedStoryCards();

    return true;
}

void StoriesPanel::setSearchingCallback(const searchingCallback& cb) {
    _searchBar->setSearchingCallback(cb);
}

void StoriesPanel::setGettingTopNCallback(const gettingTopsCallback& cb, int topN) {
    _searchBar->setGettingTopNCallback(cb, topN);
}

void StoriesPanel::addStory(Story* story) {
    auto storyCard = StoryCard::create(StoryCardEnv::PlayerStories, story);
    _storyCards->addCard(storyCard);
}

void StoriesPanel::addStories(const Vector<Story*>& stories) {
    for (auto story : stories) {
        auto storyCard = StoryCard::create(StoryCardEnv::PlayerStories, story);
        _storyCards->addCard(storyCard);
    }
}

void StoriesPanel::loadSubscribedStoryCards() {
    auto stories = loadStoriesFromPath("stories");
    addStories(stories);
}

Vector<Story*> StoriesPanel::loadStoriesFromPath(const char* path) {
    Vector<Story*> stories;
    return stories;
}

void StoriesPanel::addBattleCityStoryCard() {
    auto story = Story::create();
    story->setName("经典坦克大战");
    story->setCover("images/game-cover-battle-city.png");
    story->setDescription("80后的经典坦克大战");
    story->setSubscribed();

    addStory(story);
}
