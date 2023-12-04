#include "ToggleButton.h"

#include "ui/UILayout.h"

USING_NS_CC;

ToggleButton*
ToggleButton::create(const string& onButtonImage, const string& offButtonImage,
        const cbToggleButtonClicked& toggleOnCallback, const cbToggleButtonClicked& toggleOffCallback,
        bool isOn) {

    auto *pRet = new(std::nothrow) ToggleButton();
    if (!pRet || !pRet->init(onButtonImage, offButtonImage, toggleOnCallback, toggleOffCallback, isOn)) {
        delete pRet;
        return nullptr;
    }

    pRet->autorelease();
    return pRet;
}

bool ToggleButton::init(const string& onButtonImage, const string& offButtonImage,
        const cbToggleButtonClicked& toggleOnCallback, const cbToggleButtonClicked& toggleOffCallback,
        bool isOn) {

    if (!Button::init()) {
        return false;
    }

    _onButtonImage = onButtonImage;
    _offButtonImage = offButtonImage;
    _toggleOnCallback = toggleOnCallback;
    _toggleOffCallback = toggleOffCallback;

    addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            onClicked();
            break;
        default:
            break;
        }
    });

    setScale(0.5);  // NOTE: 通过增大字体再缩小Widget以提高文件清析度
    setOn(isOn);

    return true;
}

void ToggleButton::setOn(bool value) {
    _isOn = value;
    if (_isOn) {
        loadTextureNormal(_onButtonImage);
    } else {
        loadTextureNormal(_offButtonImage);
    }
}

void ToggleButton::onClicked() {
    if (_isOn) {
        loadTextureNormal(_offButtonImage);
        _toggleOnCallback(this);
    } else {
        loadTextureNormal(_onButtonImage);
        _toggleOffCallback(this);
    }
    _isOn = !_isOn;
}
