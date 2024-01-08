#include "GameCard.h"
#include "StoryCard.h"
#include "GameScene.h"

#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "ui/UIImageView.h"

static const char* defaultButtonBgImage = "images/icon-button-bg-48.png";
static const char* defaultFont = "fonts/simhei.ttf";
static int defaultFontSize = 12;

GameCard* GameCard::createDemo(NetworkingMode nm, GameEndpoint ep) {
    auto story = Story::create();
    story->setName("Demo Game");
    story->setCover("images/game-cover-demo.png");
    story->setDescription("Game Description Here");

    return GameCard::create(nm, ep, story, 150, 200);
}

GameCard* GameCard::create(NetworkingMode nm, GameEndpoint ep,
        Story* story, float width, float height) {

    auto *pRet = new(std::nothrow) GameCard();
    if (pRet && pRet->init(nm, ep, story, width, height)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool GameCard::init(NetworkingMode nm, GameEndpoint ep,
        Story* story, float width, float height) {

    if (!Layout::init()) return false;

    _networkingMode = nm;
    _endpoint = ep;
    _story = story;

    setContentSize(Size(width, height));
    setLayoutType(Layout::Type::VERTICAL);

    auto lpH = LinearLayoutParameter::create();
    lpH->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    auto titleLabel = Text::create(_story->getName(), defaultFont, defaultFontSize);
    titleLabel->setLayoutParameter(lpH->clone());
    addChild(titleLabel);

    auto coverImage = ImageView::create(_story->getCover());
    coverImage->setScale(0.5);
    coverImage->setLayoutParameter(lpH->clone());
    addChild(coverImage);

    if (_endpoint == GameEndpoint::LOCAL) {
        auto runningBtn = Button::create(defaultButtonBgImage);
        auto btnLabel = Label::createWithTTF("启动", defaultFont, defaultFontSize);
        runningBtn->setTitleLabel(btnLabel);
        runningBtn->setLayoutParameter(lpH->clone());
        addChild(runningBtn);
        runningBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
            switch (type)
            {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                printf("GameCard running button clicked\n");
                if (_story->isSubscribed()) {
                    Director::getInstance()->pushScene(GameScene::create(GameRuntime::create(this)));
                }
                break;
            default:
                break;
            }
        });
    } else if (_endpoint == GameEndpoint::REMOTE) {
        auto layout = Layout::create();
        layout->setLayoutType(Layout::Type::HORIZONTAL);
        layout->setLayoutParameter(lpH->clone());
        addChild(layout);

        auto joiningBtn = Button::create(defaultButtonBgImage);
        auto joiningBtnLabel = Label::createWithTTF("加入", defaultFont, defaultFontSize);
        joiningBtn->setTitleLabel(joiningBtnLabel);
        joiningBtn->setLayoutParameter(lpH->clone());
        layout->addChild(joiningBtn);
        joiningBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
            switch (type)
            {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                printf("GameCard joining button clicked\n");
                //Director::getInstance()->pushScene(GameScene::create(GameRuntime::create(this)));
                break;
            default:
                break;
            }
        });

        auto watchingBtn = Button::create(defaultButtonBgImage);
        auto watchingBtnLabel = Label::createWithTTF("观看", defaultFont, defaultFontSize);
        watchingBtn->setTitleLabel(watchingBtnLabel);
        watchingBtn->setLayoutParameter(lpH->clone());
        layout->addChild(watchingBtn);
        watchingBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
            switch (type)
            {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                printf("GameCard watching button clicked\n");
                //Director::getInstance()->pushScene(GameScene::create(GameRuntime::create(this)));
                break;
            default:
                break;
            }
        });
    }

    auto descriptionText = Text::create(_story->getDescription(), defaultFont, defaultFontSize);
    descriptionText->setLayoutParameter(lpH->clone());
    addChild(descriptionText);

    return true;
}

GameRuntime* GameRuntime::create(GameCard* gameCard) {
    auto *pRet = new(std::nothrow) GameRuntime();
    if (pRet && pRet->init()) {
        pRet->_gameCard = gameCard;
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

void GameRuntime::saveCheckPoint() {
}

void GameRuntime::loadCheckPoint() {
}
