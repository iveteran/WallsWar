#include "StackedButton.h"

#include "ui/UILayout.h"

USING_NS_CC;

StackedButton*
StackedButton::create(const std::vector<Item>& items) {

    auto *pRet = new(std::nothrow) StackedButton();
    if (!pRet || !pRet->init(items)) {
        delete pRet;
        return nullptr;
    }

    pRet->autorelease();
    return pRet;
}

bool StackedButton::init(const std::vector<Item>& items) {

    if (!Button::init() || items.empty()) {
        return false;
    }

    _items = items;
    auto item = _items[_currentIndex];
    loadTextureNormal(item.icon);

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

    return true;
}

int StackedButton::getCurrentIndex() const {
    return _currentIndex;
}

const StackedButton::Item*
StackedButton::getCurrentItem() const {
    return getItem(_currentIndex);
}

const StackedButton::Item*
StackedButton::getItem(int index) const {
    return (index >= 0 && index < _items.size()) ? &(_items[index]) : nullptr;
}

void StackedButton::onClicked() {
    auto item = _items[_currentIndex];
    if (item.callback) {
        item.callback(this, _currentIndex);
    }
    shift();
}

void StackedButton::shift() {
    _currentIndex = (_currentIndex + 1) % _items.size();
    replaceIcon();
}

void StackedButton::select(int index) {
    if (index < 0 || index >= _items.size() || index == _currentIndex) {
        return;
    }
    _currentIndex = index;
    replaceIcon();
}

void StackedButton::replaceIcon() {
    auto item = _items[_currentIndex];
    loadTextureNormal(item.icon);
}
