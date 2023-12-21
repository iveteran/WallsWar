#pragma once

#include "cocos2d.h"
#include "ui/UIListView.h"

USING_NS_CC;

using namespace cocos2d::ui;

class CardsView : public ListView {
public:
    static CardsView* create(float width, float height);
    bool init(float width, float height);

    void addCard(Widget* card);
};
