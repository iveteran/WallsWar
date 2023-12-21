#include "GameCard.h"
#include "StoryCard.h"
#include "GameScene.h"

#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "ui/UIImageView.h"

static const char* defaultButtonBgImage = "images/icon-button-bg-48.png";
static const char* defaultFont = "fonts/simhei.ttf";
static int defaultFontSize = 12;

GameCard* GameCard::createDemo() {
    auto story = Story::create();
    story->setName("Demo Game");
    story->setCover("images/game-cover-demo.png");
    story->setDescription("Game Description Here");

    return GameCard::create(story, 150, 200);
}

GameCard* GameCard::create(Story* story, float width, float height) {
    auto *pRet = new(std::nothrow) GameCard();
    if (pRet && pRet->init(story, width, height)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool GameCard::init(Story* story, float width, float height) {
    if (!Layout::init()) return false;

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

    if (_story->isSubscribed()) {
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
                Director::getInstance()->pushScene(GameScene::createScene());
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
