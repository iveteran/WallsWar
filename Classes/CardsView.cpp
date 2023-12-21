#include "CardsView.h"

CardsView* CardsView::create(float width, float height) {
    auto pRet = new(std::nothrow) CardsView();
    if (pRet && pRet->init(width, height)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool CardsView::init(float width, float height) {
    if (!ListView::init()) {
        return false;
    }

    setContentSize(Size(width, height));
    setDirection(ListView::Direction::HORIZONTAL);
    setBounceEnabled(true);
    setGravity(ListView::Gravity::CENTER_VERTICAL);
    setScrollBarEnabled(false);
    forceDoLayout();

    return true;
}

void CardsView::addCard(Widget* card) {
    pushBackCustomItem(card);
}
