#include "CreatingGameScene.h"
#include "UISceneTitlePanel.h"
#include "StoriesPanel.h"
#include "StoryMarketPanel.h"

static const Color3B defaultBgColor = Color3B(41, 74, 122);  // 深蓝色
static int defaultOpacity = 128;

CreatingGameScene* CreatingGameScene::create(NetworkingMode nm) {
    auto pRet = new(std::nothrow) CreatingGameScene();
    if (pRet && pRet->init(nm)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool CreatingGameScene::init(NetworkingMode nm) {
    if (!Scene::init()) {
        return false;
    }
    _networkingMode = nm;

    auto winSize = Director::getInstance()->getVisibleSize();

    auto layout = Layout::create();
    layout->setContentSize(winSize);
    layout->setLayoutType(Layout::Type::VERTICAL);
    layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    layout->setBackGroundColor(defaultBgColor);
    layout->setBackGroundColorOpacity(defaultOpacity);
    addChild(layout);

    auto lpH = LinearLayoutParameter::create();
    lpH->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    int titlePanelHeight = UISceneTitlePanel::HEIGHT;
    auto title = StringUtils::format("创建游戏-%s", getNetworkingModeName(_networkingMode));
    auto titlePanel = UISceneTitlePanel::create(winSize.width, titlePanelHeight, title,
            "故事市场", CC_CALLBACK_1(CreatingGameScene::onGameMarketButtonClicked, this));
    titlePanel->setLayoutParameter(lpH->clone());
    layout->addChild(titlePanel);

    auto storiesPanel = StoriesPanel::create(winSize.width, winSize.height - titlePanelHeight, _networkingMode);
    storiesPanel->setSearchingCallback(CC_CALLBACK_2(CreatingGameScene::doSearch, this));
    //storiesPanel->setGettingTopNCallback(CC_CALLBACK_2(CreatingGameScene::getTopN, this), 10);
    storiesPanel->setLayoutParameter(lpH->clone());
    layout->addChild(storiesPanel);

    return true;
}

void CreatingGameScene::onGameMarketButtonClicked(const UISceneTitlePanel* titlePanel) {
    printf("CreatingGameScene story market button clicked\n");
    openStoryMarketDialog();
}

void CreatingGameScene::openStoryMarketDialog() {
    auto winSize = Director::getInstance()->getVisibleSize();
    _storyMarketDialog = StoryMarketPanel::create(winSize.width * 0.8, winSize.height * 0.8, _networkingMode);
    _storyMarketDialog->setClosedCallback(CC_CALLBACK_1(CreatingGameScene::onCloseStoryMarketDailog, this));
    addChild(_storyMarketDialog);
}

void CreatingGameScene::onCloseStoryMarketDailog(PopupLayer* dialog) {
    removeChild(_storyMarketDialog);
    _storyMarketDialog = nullptr;
}

void CreatingGameScene::doSearch(const SearchBar* sb, const char* keywords) {
}

//void CreatingGameScene::getTopN(const SearchBar* sb, int topN) {
//}
