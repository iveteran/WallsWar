#include "GameStatsPanel.h"
#include "MapLayer.h"
#include "Camp.h"
#include "GameCard.h"

#include "ui/UIListView.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"

using namespace cocos2d::ui;

static const char* defaultButtonBgImage = "images/icon-button-bg-48.png";
static const char* defaultFont = "fonts/simhei.ttf";
static const int defaultFontSize = 12;
static const float defaultFontScale = 0.5f;
static const Color3B defaultBackgroundColor = Color3B(51, 51, 51); // near gray
static const Color3B colorSteelBlue = Color3B(70,130,180);     // steel blue
static const int defaultOpacity = 200;
static const int titleAndButtonHeight = 60;

GameStatsPanel*
GameStatsPanel::create(const GameRuntime* gameRuntime) {
    auto *pRet = new(std::nothrow) GameStatsPanel();
    if (pRet && pRet->init(gameRuntime)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool GameStatsPanel::init(const GameRuntime* gameRuntime) {
    if (!Layout::init())
        return false;
    _gameRuntime = gameRuntime;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    setContentSize(visibleSize * 0.8);
    setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    setBackGroundColor(defaultBackgroundColor);
    setBackGroundColorOpacity(defaultOpacity);

    setLayoutType(Layout::Type::VERTICAL);

    auto lp = LinearLayoutParameter::create();
    lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    auto title = (gameRuntime->getStatus() == GameStatus::RUNNING ? "Gaming in progress" :
        (StringUtils::format("Your camp are %s!", gameRuntime->getStatus() == GameStatus::WIN ? "WIN" : "LOST")));
    auto titleLabel = Text::create(title, defaultFont, defaultFontSize);
    titleLabel->setLayoutParameter(lp->clone());
    addChild(titleLabel);

    createStatsTable();

    createButtons();

    return true;
}

void GameStatsPanel::createStatsTable() {
    auto camps = MapLayer::getInstance()->getCamps();
    printf(">>> num camp: %d\n", camps.size());

    float spacing = 4;
    auto campsView = ListView::create();
    campsView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    campsView->setBounceEnabled(true);
    campsView->setScrollBarEnabled(false);
    campsView->setContentSize(Size(getContentSize().width - (spacing * camps.size()),
                getContentSize().height - titleAndButtonHeight));
    campsView->setGravity(ListView::Gravity::CENTER_VERTICAL);
    campsView->setItemsMargin(spacing);
    campsView->forceDoLayout();

    auto campStatsCardSize = Size(campsView->getContentSize().width / camps.size(),
            getContentSize().height - titleAndButtonHeight);
    for (auto camp : camps) {
        auto campStatsCard = buildCampStatsCard(camp, campStatsCardSize);
        campsView->pushBackCustomItem(campStatsCard);
    }

    auto lp = LinearLayoutParameter::create();
    lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    campsView->setLayoutParameter(lp);
    addChild(campsView);
}

Layout* GameStatsPanel::buildCampStatsCard(const Camp* camp, const Size& cardSize) {
    /*
    auto campStatsCard = ListView::create();
    campStatsCard->setDirection(ui::ScrollView::Direction::VERTICAL);
    campStatsCard->setBounceEnabled(true);
    campStatsCard->setScrollBarEnabled(false);
    campStatsCard->setContentSize(cardSize);
    campStatsCard->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    campStatsCard->setItemsMargin(spacing);
    campStatsCard->forceDoLayout();

    campStatsCard->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    campStatsCard->setBackGroundColor(colorSteelBlue);
    campStatsCard->setBackGroundColorOpacity(128);

    auto players = camp->getPlayers();
    printf(">>> num players: %d\n", players.size());
    if (players.size() > 0) {
        for (auto iter : players) {
            auto player = iter.second;
            auto rowString = StringUtils::format("score: %d", player->getScore());
            auto itemLabel = Text::create(rowString, defaultFont, defaultFontSize);
            campStatsCard->pushBackCustomItem(itemLabel);
        }
    } else {
        // demo code
        auto rowString = StringUtils::format("score: %d", 1024);
        auto itemLabel = Text::create(rowString, defaultFont, defaultFontSize);
        campStatsCard->pushBackCustomItem(itemLabel);

        auto rowString2 = StringUtils::format("score: %d", 2048);
        auto itemLabel2 = Text::create(rowString2, defaultFont, defaultFontSize);
        campStatsCard->pushBackCustomItem(itemLabel2);
    }
    */

    auto campStatsCard = Layout::create();
    campStatsCard->setLayoutType(Layout::Type::VERTICAL);
    campStatsCard->setContentSize(cardSize);
    campStatsCard->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    campStatsCard->setBackGroundColor(colorSteelBlue);
    campStatsCard->setBackGroundColorOpacity(128);

    auto lp = LinearLayoutParameter::create();
    lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    auto players = camp->getPlayers();
    printf(">>> num players: %d\n", players.size());
    if (players.size() > 0) {
        for (auto iter : players) {
            auto player = iter.second;
            auto rowString = StringUtils::format("score: %d", player->getScore());
            auto itemLabel = Text::create(rowString, defaultFont, defaultFontSize);
            itemLabel->setLayoutParameter(lp->clone());
            campStatsCard->addChild(itemLabel);
        }
    } else {
        auto rowString = StringUtils::format("score: %d", 1024);
        auto itemLabel = Text::create(rowString, defaultFont, defaultFontSize);
        itemLabel->setLayoutParameter(lp->clone());
        campStatsCard->addChild(itemLabel);

        auto rowString2 = StringUtils::format("score: %d", 2048);
        auto itemLabel2 = Text::create(rowString2, defaultFont, defaultFontSize);
        itemLabel2->setLayoutParameter(lp->clone());
        campStatsCard->addChild(itemLabel2);
    }

    return campStatsCard;
}

void GameStatsPanel::createButtons() {
    auto layout = Layout::create();
    layout->setContentSize(Size(100, 30));
    layout->setLayoutType(Layout::Type::HORIZONTAL);
    auto lp0 = LinearLayoutParameter::create();
    lp0->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout->setLayoutParameter(lp0);
    addChild(layout);

    auto lp = LinearLayoutParameter::create();
    lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    float spacing = 2;
    lp->setMargin(Margin(spacing, 0, spacing, 0));

    auto returnBtn = Button::create(defaultButtonBgImage);
    auto returnBtnLabel = Label::createWithTTF("返回主菜单", defaultFont, defaultFontSize);
    returnBtnLabel->setScale(0.7);
    returnBtn->setTitleLabel(returnBtnLabel);
    returnBtn->setLayoutParameter(lp->clone());
    layout->addChild(returnBtn);
    returnBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            if (_clickedCallback) {
                _clickedCallback();
            }
            break;
        default:
            break;
        }
    });

    auto nextStageBtn = Button::create(defaultButtonBgImage);
    auto nextStageBtnLabel = Label::createWithTTF("下一关卡", defaultFont, defaultFontSize);
    nextStageBtnLabel->setScale(0.7);
    nextStageBtn->setTitleLabel(nextStageBtnLabel);
    nextStageBtn->setLayoutParameter(lp->clone());
    layout->addChild(nextStageBtn);
    nextStageBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            if (_clickedNextStageCallback) {
                _clickedNextStageCallback();
            }
            break;
        default:
            break;
        }
    });
}

void GameStatsPanel::setClickedCallback(const clickedCallback& cb) {
    _clickedCallback = cb;
}

void GameStatsPanel::setClickedNextStageCallback(const clickedCallback& cb) {
    _clickedNextStageCallback = cb;
}
