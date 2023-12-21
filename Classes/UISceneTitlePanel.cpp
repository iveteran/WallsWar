#include "UISceneTitlePanel.h"

#include "ui/UIText.h"
#include "ui/UIButton.h"

static const char* defaultButtonBgImage = "images/icon-button-bg-48.png";
static const char* defaultFont = "fonts/simhei.ttf";
static int defaultFontSize = 12;
static float defaultFontScale = 0.5f;

UISceneTitlePanel* UISceneTitlePanel::create(float width, float height, const std::string& title,
        const char* customButtonName, const customButtonClickedCallback& customButtonCb) {
    auto pRet = new(std::nothrow) UISceneTitlePanel();
    if (pRet && pRet->init(width, height, title, customButtonName, customButtonCb)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool UISceneTitlePanel::init(float width, float height, const std::string& title,
        const char* customButtonName, const customButtonClickedCallback& customButtonCb) {
    if (!Layout::init()) return false;

    _customButtonCb = customButtonCb;

    setContentSize(Size(width, height));
    setLayoutType(Layout::Type::RELATIVE);

    _goBackBtn = Button::create("images/icon-goback.png");
    auto lp1 = RelativeLayoutParameter::create();
    lp1->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    _goBackBtn->setLayoutParameter(lp1);
    addChild(_goBackBtn);
    _goBackBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("UISceneTitlePanel go back button clicked\n");
            Director::getInstance()->popScene();
            break;
        default:
            break;
        }
    });

    auto titleLabel = Text::create(title, defaultFont, defaultFontSize);
    auto lp2 = RelativeLayoutParameter::create();
    lp2->setAlign(RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    titleLabel->setLayoutParameter(lp2);
    addChild(titleLabel);

    if (customButtonName) {
        auto customBtn = Button::create(defaultButtonBgImage);
        auto customBtnLabel = Label::createWithTTF(customButtonName, defaultFont, defaultFontSize);
        customBtn->setTitleLabel(customBtnLabel);
        auto lp3 = RelativeLayoutParameter::create();
        lp3->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
        customBtn->setLayoutParameter(lp3);
        addChild(customBtn);
        customBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
            switch (type)
            {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                printf("UISceneTitlePanel custom button clicked\n");
                if (_customButtonCb) {
                    _customButtonCb(this);
                }
                break;
            default:
                break;
            }
        });
    }

    return true;
}

void UISceneTitlePanel::disableGoBackButton() {
    _goBackBtn->setVisible(false);
}
