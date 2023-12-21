#include "StoryMarketPanel.h"
#include "StoryCard.h"
#include "CardsView.h"
#include "SearchBar.h"

static const Color3B defaultBgColor = Color3B(41, 74, 122);  // 深蓝色
static int defaultOpacity = 255;

StoryMarketPanel* StoryMarketPanel::create(float width, float height, NetworkingMode nm) {
    auto pRet = new(std::nothrow) StoryMarketPanel();
    if (pRet && pRet->init(width, height, nm)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool StoryMarketPanel::init(float width, float height, NetworkingMode nm) {
    if (!PopupLayer::initWithModal(width, height, defaultBgColor, defaultOpacity)) {
        return false;
    }
    _networkingMode = nm;

    setTitle("故事市场");

    auto layout = Layout::create();
    layout->setContentSize(Size(width, height - 10));
    layout->setLayoutType(Layout::Type::VERTICAL);
    addMainPanel(layout);

    auto lpH = LinearLayoutParameter::create();
    lpH->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    auto searchBar = SearchBar::create();
    searchBar->setSearchingCallback(CC_CALLBACK_2(StoryMarketPanel::doSearch, this));
    searchBar->setGettingTopNCallback(CC_CALLBACK_2(StoryMarketPanel::getTopN, this), 10);
    searchBar->setLayoutParameter(lpH->clone());
    layout->addChild(searchBar);

    _storyCards = CardsView::create(width, height * 0.8);
    _storyCards->setLayoutParameter(lpH->clone());
    layout->addChild(_storyCards);

    addBattleCityStoryCard();
    //loadStories();

    return true;
}

void StoryMarketPanel::doSearch(const SearchBar* sb, const char* keywords) {
}

void StoryMarketPanel::getTopN(const SearchBar* sb, int topN) {
}

void StoryMarketPanel::loadStories() {
}

void StoryMarketPanel::subscribeStory(int storyId) {
}

void StoryMarketPanel::addStoryCard(Story* story) {
    auto storyCard = StoryCard::create(StoryCardEnv::StoryMarket, story);
    _storyCards->addCard(storyCard);
}

void StoryMarketPanel::addBattleCityStoryCard() {
    auto story = Story::create();
    story->setName("经典坦克大战");
    story->setCover("images/game-cover-battle-city.png");
    story->setDescription("80后的经典坦克大战");
    story->setSubscribed();

    addStoryCard(story);
}

void StoryMarketPanel::addDemoStoryCard() {
    auto story = Story::create();
    story->setName("Demo Story");
    story->setCover("images/game-cover-demo.png");
    story->setDescription("This is story description");

    addStoryCard(story);
}
