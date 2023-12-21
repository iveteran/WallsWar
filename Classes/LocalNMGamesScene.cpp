#include "LocalNMGamesScene.h"
#include "GameCard.h"
#include "CardsView.h"
#include "UISceneTitlePanel.h"
#include "PlayerGamesScene.h"

#include "ui/UILayout.h"
#include "ui/UILoadingBar.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "ui/UIImageView.h"

static const int PERCENT_100 = 100;

static const char* defaultButtonBgImage = "images/icon-button-bg-48.png";
static const char* defaultFont = "fonts/simhei.ttf";
static int defaultFontSize = 12;
static float defaultFontScale = 0.5f;
static const Color3B defaultBgColor = Color3B(41, 74, 122);  // 深蓝色
static int defaultOpacity = 128;

bool LocalNMGamesScene::init() {
    if (!Scene::init()) {
        return false;
    }
    auto winSize = Director::getInstance()->getVisibleSize();

    auto layout = Layout::create();
    layout->setContentSize(winSize);
    layout->setLayoutType(Layout::Type::RELATIVE);
    addChild(layout);

    layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    layout->setBackGroundColor(defaultBgColor);
    layout->setBackGroundColorOpacity(defaultOpacity);

    int titlePanelHeight = UISceneTitlePanel::HEIGHT;
    auto titlePanel = UISceneTitlePanel::create(winSize.width, titlePanelHeight, "本地网络游戏",
            "我的游戏", CC_CALLBACK_1(LocalNMGamesScene::onMyGamesButtonClicked, this));
    auto lp1 = RelativeLayoutParameter::create();
    lp1->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    titlePanel->setLayoutParameter(lp1);
    layout->addChild(titlePanel);

    auto scanningPanel = buildScanningPanel(winSize.width * 0.3, winSize.height - titlePanelHeight);
    auto lp2 = RelativeLayoutParameter::create();
    lp2->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    scanningPanel->setLayoutParameter(lp2);
    layout->addChild(scanningPanel);

    _gameCards = CardsView::create(winSize.width * 0.7, winSize.height - titlePanelHeight);
    auto lp3 = RelativeLayoutParameter::create();
    lp3->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
    _gameCards->setLayoutParameter(lp3);
    layout->addChild(_gameCards);

    scheduleUpdate();

    return true;
}

Layout* LocalNMGamesScene::buildScanningPanel(float width, float height) {
    auto layout = Layout::create();
    layout->setContentSize(Size(width, height));
    layout->setLayoutType(Layout::Type::VERTICAL);

    auto lpH = LinearLayoutParameter::create();
    lpH->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    auto scanningAnimation = ImageView::create("images/image-game-scanning.png");
    scanningAnimation->setLayoutParameter(lpH->clone());
    layout->addChild(scanningAnimation);

    _scanningProgressBar = LoadingBar::create("images/cocosui/sliderProgress.png");
    _scanningProgressBar->setLayoutParameter(lpH->clone());
    layout->addChild(_scanningProgressBar);

    _elapsedLabel = Text::create();
    _elapsedLabel->setLayoutParameter(lpH->clone());
    layout->addChild(_elapsedLabel);

    _reScanningBtn = Button::create(defaultButtonBgImage);
    auto btnLabel = Label::createWithTTF("重新扫描", defaultFont, defaultFontSize);
    _reScanningBtn->setTitleLabel(btnLabel);
    _reScanningBtn->setLayoutParameter(lpH->clone());
    _reScanningBtn->setVisible(false);
    layout->addChild(_reScanningBtn);

    return layout;
}

void LocalNMGamesScene::onMyGamesButtonClicked(const UISceneTitlePanel* titlePanel) {
    printf("LocalNMGamesScene my games button clicked\n");
    Director::getInstance()->pushScene(PlayerGamesScene::create(NetworkingMode::LOCAL));
}

void LocalNMGamesScene::addDemoGameCard() {
    auto gameCard = GameCard::createDemo();
    _gameCards->addCard(gameCard);
}

void LocalNMGamesScene::update(float delta) {
    _elapsed += delta;
    _secondTick += delta;
    float loadingPercent = _scanningProgressBar->getPercent();
    if (loadingPercent < PERCENT_100 && _secondTick >= 1.0f) {
        loadingPercent += 10;
        _scanningProgressBar->setPercent(loadingPercent);
        _secondTick = 0.f;

        // update loading info
        auto loadingInfo = StringUtils::format("Loading %0.2f, elapsed: %0.2f", loadingPercent, _elapsed);
        _elapsedLabel->setString(loadingInfo);

        addDemoGameCard();
    }
    if (loadingPercent >= PERCENT_100) {
        _reScanningBtn->setVisible(true);
    }
}
