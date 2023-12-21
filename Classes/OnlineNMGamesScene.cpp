#include "OnlineNMGamesScene.h"
#include "UISceneTitlePanel.h"
#include "GameCard.h"
#include "CardsView.h"
#include "SearchBar.h"
#include "PlayerGamesScene.h"

static const Color3B defaultBgColor = Color3B(41, 74, 122);  // 深蓝色
static int defaultOpacity = 128;

bool OnlineNMGamesScene::init() {
    if (!Scene::init()) {
        return false;
    }
    auto winSize = Director::getInstance()->getVisibleSize();

    auto layout = Layout::create();
    layout->setContentSize(winSize);
    layout->setLayoutType(Layout::Type::VERTICAL);
    addChild(layout);

    layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    layout->setBackGroundColor(defaultBgColor);
    layout->setBackGroundColorOpacity(defaultOpacity);

    auto lpH = LinearLayoutParameter::create();
    lpH->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    auto titlePanel = UISceneTitlePanel::create(winSize.width, UISceneTitlePanel::HEIGHT, "线上网络游戏",
            "我的游戏", CC_CALLBACK_1(OnlineNMGamesScene::onMyGamesButtonClicked, this));
    titlePanel->setLayoutParameter(lpH->clone());
    layout->addChild(titlePanel);

    auto searchBar = SearchBar::create();
    searchBar->setSearchingCallback(CC_CALLBACK_2(OnlineNMGamesScene::doSearch, this));
    searchBar->setGettingTopNCallback(CC_CALLBACK_2(OnlineNMGamesScene::getTopN, this), 10);
    searchBar->setLayoutParameter(lpH->clone());
    layout->addChild(searchBar);

    _gameCards = CardsView::create(winSize.width, winSize.height * 0.7);
    _gameCards->setLayoutParameter(lpH->clone());
    layout->addChild(_gameCards);

    return true;
}

void OnlineNMGamesScene::onMyGamesButtonClicked(const UISceneTitlePanel* titlePanel) {
    printf("OnlineNMGamesScene my games button clicked\n");
    Director::getInstance()->pushScene(PlayerGamesScene::create(NetworkingMode::ONLINE));
}

void OnlineNMGamesScene::doSearch(const SearchBar* sb, const char* keywords) {
}

void OnlineNMGamesScene::getTopN(const SearchBar* sb, int topN) {
    addDemoGameCard();
}

void OnlineNMGamesScene::addDemoGameCard() {
    auto gameCard = GameCard::createDemo();
    _gameCards->addCard(gameCard);
}
