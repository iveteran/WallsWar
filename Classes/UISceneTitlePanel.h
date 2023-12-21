#pragma once

#include "cocos2d.h"
#include "ui/UILayout.h"

USING_NS_CC;
namespace cocos2d::ui {
    class Layout;
    class Button;
}
using namespace cocos2d::ui;

class UISceneTitlePanel;

using customButtonClickedCallback = std::function<void(const UISceneTitlePanel*)>;

class UISceneTitlePanel : public Layout {
public:
    static const int HEIGHT = 20;

public:
    static UISceneTitlePanel* create(float width, float height, const std::string& title,
            const char* customButtonName, const customButtonClickedCallback& customButtonCb);
    bool init(float width, float height, const std::string& title,
        const char* customButtonName, const customButtonClickedCallback& customButtonCb);

    void disableGoBackButton();

private:
    Button* _goBackBtn = nullptr;
    customButtonClickedCallback _customButtonCb = nullptr;
};
