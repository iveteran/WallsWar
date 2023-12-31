#include "NetworkingModeMenuScene.h"
#include "SingleNMGamesScene.h"
#include "LocalNMGamesScene.h"
#include "OnlineNMGamesScene.h"
#include "CardsView.h"
#include "UISceneTitlePanel.h"

#include "ui/UIText.h"
#include "ui/UIImageView.h"
#include "ui/UIButton.h"

static const char* defaultButtonBgImage = "images/icon-button-bg-48.png";
static const char* defaultFont = "fonts/simhei.ttf";
static int defaultFontSize = 12;
static float defaultFontScale = 0.5f;
static const Color3B defaultBgColor = Color3B(41, 74, 122);    // 深蓝色
static const Color3B colorLightSeaGreen = Color3B(32,178,170); // light sea green
static const Color3B colorDodgerBlue = Color3B(30,144,255);    // dodger blue
static const Color3B colorSteelBlue = Color3B(70,130,180);     // steel blue
static const Color3B colorRoyalBlue = Color3B(65,105,225);     // royal blue
static int defaultOpacity = 128;

NetworkingModeCard* NetworkingModeCard::create(const char* title, const char* bgImage, const char* summary) {
    auto pRet = new(std::nothrow) NetworkingModeCard();
    if (pRet && pRet->init(title, bgImage, summary)) { 
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool NetworkingModeCard::init(const char* title, const char* bgImage, const char* summary) {
    if (!Layout::init()) return false;

    _title = title;
    _summary = summary;

    auto winSize = Director::getInstance()->getVisibleSize();
    setContentSize(Size(winSize.width / 3, winSize.height * 0.5));

    setBackGroundColor(colorSteelBlue);

    setLayoutType(Layout::Type::VERTICAL);

    auto lp = LinearLayoutParameter::create();
    lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    auto titleLabel = Text::create(title, defaultFont, defaultFontSize);
    addChild(titleLabel);
    auto lp2 = lp->clone();
    lp2->setMargin(Margin(0, 10, 0, 0));
    titleLabel->setLayoutParameter(lp2);

    auto bgImageView = ImageView::create(bgImage);
    addChild(bgImageView);
    auto lp3 = lp->clone();
    lp3->setMargin(Margin(0, 5, 0, 5));
    bgImageView->setLayoutParameter(lp3);

    _enterBtn = Button::create(defaultButtonBgImage);
    auto enterBtnLabel = Label::createWithTTF("进入", defaultFont, defaultFontSize);
    //enterBtnLabel->setColor(Color3B::BLUE);
    _enterBtn->setTitleLabel(enterBtnLabel);
    //_enterBtn->setTitleColor(Color3B::BLUE);
    addChild(_enterBtn);
    _enterBtn->setLayoutParameter(lp->clone());
    _enterBtn->setVisible(false);

    _enterBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("NetworkingModeCard enter button clicked\n");
            _onEnteringCallback(this);
            break;
        default:
            break;
        }
    });

    setTouchEnabled(true);
    addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("NetworkingModeCard clicked\n");
            _onSelectedCallback(this);
            break;
        default:
            break;
        }
    });


    return true;
}

void NetworkingModeCard::setSelectedCallback(const onSelectedCallback& cb) {
    _onSelectedCallback = cb;
}

void NetworkingModeCard::setEnteringCallback(const onEnteringCallback& cb) {
    _onEnteringCallback = cb;
}

void NetworkingModeCard::setActive() {
    setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _enterBtn->setVisible(true);
}

void NetworkingModeCard::setUnactive() {
    setBackGroundColorType(Layout::BackGroundColorType::NONE);
    _enterBtn->setVisible(false);
}

bool NetworkingModeMenuScene::init() {
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

    auto titlePanel = UISceneTitlePanel::create(winSize.width, UISceneTitlePanel::HEIGHT, "游戏网络模式选择",
            "最近历史", CC_CALLBACK_1(NetworkingModeMenuScene::onHistoryButtonClicked, this));
    titlePanel->disableGoBackButton();
    titlePanel->setLayoutParameter(lpH->clone());
    layout->addChild(titlePanel);

    _nmCards = buildNMCards(winSize.width, winSize.height * 0.6);
    _nmCards->setLayoutParameter(lpH->clone());
    layout->addChild(_nmCards);

    _summaryPanel = buildSummaryPanel(winSize.width, winSize.height * 0.3);
    _summaryPanel->setLayoutParameter(lpH->clone());
    layout->addChild(_summaryPanel);

    return true;
}

CardsView* NetworkingModeMenuScene::buildNMCards(float width, float height) {
    auto nmCards = CardsView::create(width, height);

    auto singleModeCard = NetworkingModeCard::create("单机", "images/networking-mode-single.png", "summary: single mode");
    singleModeCard->setSelectedCallback(CC_CALLBACK_1(NetworkingModeMenuScene::onItemSelected, this));
    singleModeCard->setEnteringCallback(CC_CALLBACK_1(NetworkingModeMenuScene::onEnterSingleMode, this));
    nmCards->addCard(singleModeCard);

    auto localModeCard = NetworkingModeCard::create("本地联网", "images/networking-mode-local.png", "summary: local mode");
    localModeCard->setSelectedCallback(CC_CALLBACK_1(NetworkingModeMenuScene::onItemSelected, this));
    localModeCard->setEnteringCallback(CC_CALLBACK_1(NetworkingModeMenuScene::onEnterLocalMode, this));
    nmCards->addCard(localModeCard);

    auto onlineModeCard = NetworkingModeCard::create("线上联网", "images/networking-mode-online.png", "summary: online mode");
    onlineModeCard->setSelectedCallback(CC_CALLBACK_1(NetworkingModeMenuScene::onItemSelected, this));
    onlineModeCard->setEnteringCallback(CC_CALLBACK_1(NetworkingModeMenuScene::onEnterOnlineMode, this));
    nmCards->addCard(onlineModeCard);

    return nmCards;
}

Layout* NetworkingModeMenuScene::buildSummaryPanel(float width, float height) {
    auto layout = Layout::create();
    layout->setLayoutType(Layout::Type::RELATIVE);
    layout->setContentSize(Size(width, height));
    layout->setBackGroundColor(colorSteelBlue);

    _summaryLabel = Text::create("", defaultFont, defaultFontSize);
    layout->addChild(_summaryLabel);
    auto lp = RelativeLayoutParameter::create();
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    _summaryLabel->setLayoutParameter(lp);

    return layout;
}

void NetworkingModeMenuScene::onItemSelected(NetworkingModeCard* nmCard) {
    auto nmCards = _nmCards->getItems();
    for (auto item : nmCards) {
        auto nmCard = dynamic_cast<NetworkingModeCard*>(item);
        nmCard->setUnactive();
    }
    nmCard->setActive();
    _selectedNMCard = nmCard;

    _summaryLabel->setString(nmCard->getSummary());
    _summaryPanel->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
}

void NetworkingModeMenuScene::onEnterSingleMode(NetworkingModeCard* nmCard) {
    Director::getInstance()->pushScene(SingleNMGamesScene::create());
}

void NetworkingModeMenuScene::onEnterLocalMode(NetworkingModeCard* nmCard) {
    Director::getInstance()->pushScene(LocalNMGamesScene::create());
}

void NetworkingModeMenuScene::onEnterOnlineMode(NetworkingModeCard* nmCard) {
    Director::getInstance()->pushScene(OnlineNMGamesScene::create());
}

void NetworkingModeMenuScene::onHistoryButtonClicked(const UISceneTitlePanel* titlePanel) {
}
