#include "StoryCard.h"

#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "ui/UIImageView.h"

static const char* defaultButtonBgImage = "images/icon-button-bg-48.png";
static const char* defaultFont = "fonts/simhei.ttf";
static int defaultFontSize = 12;

StoryCard* StoryCard::create(StoryCardEnv env, Story* story) { 
    auto *pRet = new(std::nothrow) StoryCard(); 
    if (pRet && pRet->init(env, story)) {
        pRet->autorelease(); 
        return pRet; 
    } else {
        delete pRet; 
        return nullptr; 
    }
}

bool StoryCard::init(StoryCardEnv env, Story* story) {
    if (!Layout::init() || story == nullptr) return false;

    _env = env;
    _story = story;

    setContentSize(Size(100, 120));

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

    if (_env == StoryCardEnv::PlayerStories) {
        auto creatingBtn = Button::create(defaultButtonBgImage);
        auto btnLabel = Label::createWithTTF("创建游戏", defaultFont, defaultFontSize);
        creatingBtn->setTitleLabel(btnLabel);
        creatingBtn->setLayoutParameter(lpH->clone());
        addChild(creatingBtn);
        creatingBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
            switch (type)
            {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                printf("StoryCard creating button clicked\n");
                break;
            default:
                break;
            }
        });
    } else {
        if (_story->isSubscribed()) {
            //auto creatingBtn = Button::create(defaultButtonBgImage);
            //auto btnLabel = Label::createWithTTF("已订购", defaultFont, defaultFontSize);
            //creatingBtn->setTitleLabel(btnLabel);
            //creatingBtn->setLayoutParameter(lpH->clone());
            //addChild(creatingBtn);
        } else {
            auto subsBtn = Button::create(defaultButtonBgImage);
            auto btnLabel = Label::createWithTTF("订购", defaultFont, defaultFontSize);
            subsBtn->setTitleLabel(btnLabel);
            subsBtn->setLayoutParameter(lpH->clone());
            addChild(subsBtn);
            subsBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
                switch (type)
                {
                case Widget::TouchEventType::BEGAN:
                    break;
                case Widget::TouchEventType::ENDED:
                    printf("StoryCard subscribe button clicked\n");
                    break;
                default:
                    break;
                }
            });
        }
    }

    auto descriptionText = Text::create(_story->getDescription(), defaultFont, defaultFontSize);
    descriptionText->setLayoutParameter(lpH->clone());
    addChild(descriptionText);

    return true;
}
