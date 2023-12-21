#include "SearchBar.h"

#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "ui/UIEditBox/UIEditBox.h"

static const char* defaultButtonBgImage = "images/icon-button-bg-48.png";
static const char* defaultFont = "fonts/simhei.ttf";
static int defaultFontSize = 12;
static float defaultFontScale = 0.5f;

SearchBar* SearchBar::create(float width, float height) {
    auto pRet = new(std::nothrow) SearchBar();
    if (pRet && pRet->init(width, height)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool SearchBar::init(float width, float height) {
    if (!Layout::init()) {
        return false;
    }

    setContentSize(Size(width, height));
    setLayoutType(Layout::Type::HORIZONTAL);

    auto lpV = LinearLayoutParameter::create();
    lpV->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);

    Size editBoxSize(width * 0.8, height * 0.9);
    _inputField = EditBox::create(editBoxSize, "");
    //_inputField->setScale(defaultFontScale);
    _inputField->setMaxLength(1000);
    _inputField->setFont(defaultFont, defaultFontSize);
    _inputField->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _inputField->setPlaceHolder("在此输入消息");
    _inputField->setPlaceholderFont(defaultFont, defaultFontSize - 2);
    //_inputField->setDelegate(this);
    _inputField->setLayoutParameter(lpV->clone());
    addChild(_inputField);

    auto searchBtn = Button::create(defaultButtonBgImage);
    auto searchBtnLabel = Label::createWithTTF("搜索", defaultFont, defaultFontSize);
    searchBtn->setTitleLabel(searchBtnLabel);
    searchBtn->setLayoutParameter(lpV->clone());
    addChild(searchBtn);
    searchBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("SearchBar search button clicked\n");
            if (_searchingCb) {
                _searchingCb(this, _inputField->getText());
            }
            break;
        default:
            break;
        }
    });

    auto topNName = StringUtils::format("Top %d", _topN);
    _topNBtn = Button::create(defaultButtonBgImage);
    _topNBtnLabel = Label::createWithTTF(topNName, defaultFont, defaultFontSize);
    _topNBtn->setTitleLabel(_topNBtnLabel);
    _topNBtn->setLayoutParameter(lpV->clone());
    addChild(_topNBtn);
    _topNBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("SearchBar top N button clicked\n");
            if (_gettingTopsCb) {
                _gettingTopsCb(this, _topN);
            }
            break;
        default:
            break;
        }
    });
    _topNBtn->setVisible(false);

    return true;
}

void SearchBar::setSearchingCallback(const searchingCallback& cb) {
    if (!cb) return;

    _searchingCb = cb;
}

void SearchBar::setGettingTopNCallback(const gettingTopsCallback& cb, int topN) {
    if (!cb || topN == 0) return;

    _gettingTopsCb = cb;
    _topN = topN;

    auto topNName = StringUtils::format("Top %d", _topN);
    _topNBtnLabel->setString(topNName);
    _topNBtn->setVisible(true);
}
