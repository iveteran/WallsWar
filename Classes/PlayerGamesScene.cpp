#include "PlayerGamesScene.h"
#include "UISceneTitlePanel.h"
#include "GameCard.h"
#include "StoryCard.h"
#include "CardsView.h"
#include "SearchBar.h"
#include "CreatingGameScene.h"

static const Color3B defaultBgColor = Color3B(41, 74, 122);  // 深蓝色
static int defaultOpacity = 128;

Story* createBattleCityStoryCard();

PlayerGamesScene* PlayerGamesScene::create(NetworkingMode nmMode) {
    auto *pRet = new(std::nothrow) PlayerGamesScene();
    if (pRet && pRet->init(nmMode)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool PlayerGamesScene::init(NetworkingMode nm) {
    if (!Scene::init()) {
        return false;
    }
    _networkingMode = nm;

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

    auto title = StringUtils::format("我的游戏-%s", getNetworkingModeName(_networkingMode));
    auto titlePanel = UISceneTitlePanel::create(winSize.width, UISceneTitlePanel::HEIGHT, title,
            "创建游戏", CC_CALLBACK_1(PlayerGamesScene::onCreatingButtonClicked, this));
    titlePanel->setLayoutParameter(lpH->clone());
    layout->addChild(titlePanel);

    auto searchBar = SearchBar::create();
    searchBar->setSearchingCallback(CC_CALLBACK_2(PlayerGamesScene::doSearch, this));
    searchBar->setLayoutParameter(lpH->clone());
    layout->addChild(searchBar);

    _gameCards = CardsView::create(winSize.width, winSize.height - (UISceneTitlePanel::HEIGHT + SearchBar::HEIGHT));
    _gameCards->setLayoutParameter(lpH->clone());
    layout->addChild(_gameCards);

    if (_networkingMode == NetworkingMode::SINGLE) {
        addBattleCityGameCard();
    }

    return true;
}

void PlayerGamesScene::onCreatingButtonClicked(const UISceneTitlePanel* titlePanel) {
    Director::getInstance()->pushScene(CreatingGameScene::create(_networkingMode));
}

void PlayerGamesScene::doSearch(const SearchBar* sb, const char* keywords) {
    addDemoGameCard();
}

void PlayerGamesScene::addDemoGameCard() {
    auto gameCard = GameCard::createDemo(_networkingMode, GameEndpoint::LOCAL);
    _gameCards->addCard(gameCard);
}

void PlayerGamesScene::addBattleCityGameCard() {
    auto story = createBattleCityStoryCard();
    auto winSize = Director::getInstance()->getVisibleSize();
    auto gameCard = GameCard::create(NetworkingMode::SINGLE, GameEndpoint::LOCAL,
            story, winSize.width * 0.3, winSize.height * 0.5);
    _gameCards->addCard(gameCard);
}

Story* createBattleCityStoryCard() {
    auto story = Story::create();
    story->setName("经典坦克大战");
    story->setCover("images/game-cover-battle-city.png");
    story->setDescription("80后的经典坦克大战");
    story->setSubscribed();

    return story;
}
