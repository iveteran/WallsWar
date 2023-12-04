#include "PageViewWindow.h"

#include "ui/UIListView.h"
#include "ui/UIPageView.h"
#include "ui/UIButton.h"
#include "ui/UIImageView.h"

static const Color3B blueBackgroundColor = Color3B(85, 170, 255);  // near blue
static const int defaultOpacity = 128;
static const float defaultIconScale = 0.3f;

bool PageViewItem::init() {
    return Widget::init();
}

bool PageViewWindow::init(Direction direction, Size pageSize, Size indicatorSize,
        int numWindowPages, int indicatorWindowIndex) {
    if (!Layout::init()) {
        return false;
    }

    _reverseMode = true;
    _windowInitialPosition = WindowPosition::TAIL;
    _direction = direction;
    _pageSize = pageSize;
    _indicatorSize = indicatorSize;
    _numWindowPages = numWindowPages;
    Size windowSize, contentSize;
    if (_direction == Direction::VERTICAL) {
        windowSize.width = _pageSize.width;
        windowSize.height = _pageSize.height * _numWindowPages;

        contentSize.width = windowSize.width + _indicatorSize.width;
        contentSize.height = std::max(windowSize.height, _indicatorSize.height);
    } else {
        windowSize.width = _pageSize.width * _numWindowPages;
        windowSize.height = _pageSize.height;

        contentSize.width = std::max(windowSize.width, _indicatorSize.width);
        contentSize.height = windowSize.height + _indicatorSize.height;
    }
    setContentSize(contentSize);
    ListView::Direction itemDirection =
        (_direction == Direction::VERTICAL ? ListView::Direction::VERTICAL : ListView::Direction::HORIZONTAL);
    ListView::Gravity itemGravity =
        (_direction == Direction::VERTICAL ? ListView::Gravity::CENTER_HORIZONTAL : ListView::Gravity::CENTER_VERTICAL);
    printf(">> windowSize: (%f, %f)\n", windowSize.width, windowSize.height);

    _pages = ListView::create();
    _pages->setContentSize(windowSize);
    _pages->setDirection(itemDirection);
    _pages->setBounceEnabled(true);
    _pages->setGravity(itemGravity);
    _pages->setScrollBarEnabled(false);
    //_pages->setMagneticType(ListView::MagneticType::TOP);
    //_pages->setItemsMargin(2);
    _pages->forceDoLayout();
    _pages->addEventListener(CC_CALLBACK_2(PageViewWindow::handleScrollEvent, this));
    addChild(_pages);
#if 0
    _pages->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _pages->setBackGroundColor(blueBackgroundColor);
    _pages->setBackGroundColorOpacity(defaultOpacity);
#endif

    _windowCenterIndex = _numWindowPages % 2 == 0 ? (_numWindowPages - 1) / 2 : (_numWindowPages / 2);
    switch (_windowInitialPosition) {
        case WindowPosition::HEAD:
            _indicatorWindowIndex = 0;
            break;
        case WindowPosition::CENTER:
            _indicatorWindowIndex = _windowCenterIndex;  // at center
            break;
        case WindowPosition::TAIL:
            _indicatorWindowIndex = _numWindowPages - 1;
            break;
        default:
            break;
    }
    auto indicatorPosition = calculateIndicatorPosition(_indicatorWindowIndex);
    _indicator = ImageView::create("images/indicator-triangle-left.png");
    _indicator->setScale(defaultIconScale);
    _indicator->setPosition(indicatorPosition);
    addChild(_indicator);

    _windowCenterPosition = calculateIndicatorPosition(_windowCenterIndex);

    _positionRatioInView = Vec2(0.5, 0.5);
    _itemAnchorPoint = Vec2(0.5, 0.5);

    return true;
}

Vec2 PageViewWindow::calculateIndicatorPosition(int indicatorIndex) {
    auto contentSize = getContentSize();
    Vec2 position;
    if (_direction == Direction::VERTICAL) {
        position.x = contentSize.width - _indicatorSize.width / 2;
        position.y = contentSize.height - (_pageSize.height * indicatorIndex + _pageSize.height / 2);
    } else {
        position.x = contentSize.width - (_pageSize.width * indicatorIndex + _pageSize.width / 2);
        position.y = contentSize.height - _indicatorSize.height / 2;
    }
    return position;
}

void PageViewWindow::pushBackPage(PageViewItem* pageItem) {
    if (_reverseMode) {
        _pages->insertCustomItem(pageItem, 0); // make pages reserve
    } else {
        _pages->pushBackCustomItem(pageItem);
    }
    adjustCurrentPageIndex();

    pageItem->_clickedCallback = CC_CALLBACK_1(PageViewWindow::onPageItemClicked, this);
}

void PageViewWindow::adjustCurrentPageIndex() {
    if (getPagesCount() <= _numWindowPages) {
        return;
    }

    switch (_windowInitialPosition) {
        case WindowPosition::HEAD:
            _indicatorPageIndex = 0;
            break;
        case WindowPosition::CENTER:
            {
                int pagesCount = getPagesCount();
                _indicatorPageIndex = (pagesCount % 2 == 0 ? (pagesCount - 1) / 2 : (pagesCount / 2));  // at center
            }
            break;
        case WindowPosition::TAIL:
            _indicatorPageIndex = getPagesLastIndex();
            break;
        default:
            break;
    }
    if (_indicatorPageIndex > 0) {
        _pages->setCurSelectedIndex(_indicatorPageIndex);
        _pages->scrollToItem(_indicatorPageIndex, _positionRatioInView, _itemAnchorPoint);
    }
}

void PageViewWindow::onPageItemClicked(PageViewItem* pageItem) {
    int selectedPageIndex = _pages->getIndex(pageItem);
    printf("## page item clicked, index: %d\n", selectedPageIndex);
    printf("## page item clicked, _indicatorPageIndex: %d\n", _indicatorPageIndex);
    printf("## page item clicked, current item index: %d\n", getCurrentIndex());
    printf("## page item clicked, _indicatorWindowIndex: %d\n", _indicatorWindowIndex);

    int pageIndexDelta = _indicatorPageIndex - selectedPageIndex;
    _indicatorPageIndex = selectedPageIndex;
    if (std::abs(pageIndexDelta) < _numWindowPages) {
        auto movingDistance = getMultipleUnitsDistance(pageIndexDelta);
        _indicator->runAction(MoveBy::create(_movingDuration, movingDistance));
        _indicatorWindowIndex -= pageIndexDelta;
    } else {
        _pages->scrollToItem(_indicatorPageIndex, _positionRatioInView, _itemAnchorPoint);

        _indicator->runAction(MoveTo::create(_movingDuration, _windowCenterPosition));
        _indicatorWindowIndex = _windowCenterIndex;
    }
    printf("## page item clicked, _indicatorPageIndex(2): %d\n", _indicatorPageIndex);
    printf("## page item clicked, _indicatorWindowIndex(2): %d\n", _indicatorWindowIndex);
}

void PageViewWindow::handleScrollEvent(Ref* sender, ListView::EventType event) {
    printf("!! handleScrollEvent, event type: %d\n", event);
    printf("!! current item index: %d\n", getCurrentIndex());
    printf("!! top item index: %d (%d)\n", _pages->getIndex(_pages->getTopmostItemInCurrentView()), getHeadIndex());
    //printf("!! center item index: %d\n", getCenterIndex());
    printf("!! bottom item index: %d\n", getTailIndex());
    printf("!! indicator page index: %d\n", _indicatorPageIndex);
    printf("!! indicator window index: %d\n", _indicatorWindowIndex);
}

Vec2 PageViewWindow::getMultipleUnitsDistance(int numUnits) {
    return _direction == Direction::VERTICAL ?
        Vec2(0, _pageSize.height * numUnits) :
        Vec2(_pageSize.width * numUnits, 0);
}

Vec2 PageViewWindow::getOneUnitDistance(ScrollDirection directionFlag) {
    return _direction == Direction::VERTICAL ?
        Vec2(0, _pageSize.height * directionFlag) :
        Vec2(_pageSize.width * directionFlag, 0);
}

Vec2 PageViewWindow::getFullWindowDistance(ScrollDirection directionFlag) {
    return _direction == Direction::VERTICAL ?
        Vec2(0, _pageSize.height * (_numWindowPages - 1) * directionFlag) :
        Vec2(_pageSize.width * (_numWindowPages -1) * directionFlag, 0);
}

bool PageViewWindow::scrollForward() {
    bool isReverted = false;
    if (isIndicatorAtHead() && isPageIndexAtHead()) {
        _indicatorPageIndex = getPagesLastIndex();
        _indicatorWindowIndex = _numWindowPages - 1;

        _pages->scrollToItem(_indicatorPageIndex, _positionRatioInView, _itemAnchorPoint);
        _indicator->runAction(MoveBy::create(_movingDuration,
                    getFullWindowDistance(ScrollDirection::BACKWARD)));
        isReverted = true;
    } else {
        if (!isPageIndexAtHead()) {
            _indicatorPageIndex--;
        }

        if (isIndicatorBehindWindowCenter() || isHeadPageShowsInWindow()) {
            _indicatorWindowIndex--;

            _indicator->runAction(MoveBy::create(_movingDuration,
                        getOneUnitDistance(ScrollDirection::FORWARD)));
        } else {
            // NOTE: 如果选择的page在view window的头或尾部，滚动会将此page滚动到view window的中间，所以indicator也要移到中间
            _pages->scrollToItem(_indicatorPageIndex, _positionRatioInView, _itemAnchorPoint);

            if (isIndicatorAtHead()) {
                // move indicator to center of view window
                //_indicator->runAction(MoveBy::create(_movingDuration,
                //            getOneUnitDistance(ScrollDirection::BACKWARD)));
                _indicator->runAction(MoveTo::create(_movingDuration, _windowCenterPosition));
                //_indicatorWindowIndex++;
                _indicatorWindowIndex = _windowCenterIndex;
            }
        }
    }
    _pages->setCurSelectedIndex(_indicatorPageIndex);

    printf("up >> current page index: %d\n", getCurrentIndex());
    printf("up >> center page index: %d\n", getCenterIndex());
    printf("up >> indicator page index: %d\n", _indicatorPageIndex);
    printf("up >> indicator window index: %d\n", _indicatorWindowIndex);

    return isReverted;
}

bool PageViewWindow::scrollBackward() {
    bool isReverted = false;
    if (isIndicatorAtTail() && isPageIndexAtTail()) {
        _indicatorPageIndex = 0;
        _indicatorWindowIndex = 0;

        _pages->scrollToItem(_indicatorPageIndex, _positionRatioInView, _itemAnchorPoint);
        _indicator->runAction(MoveBy::create(_movingDuration,
                    getFullWindowDistance(ScrollDirection::FORWARD)));
        isReverted = true;
    } else {
        if (!isPageIndexAtTail()) {
            _indicatorPageIndex++;
        }

        if (isIndicatorBeforeWindowCenter() || isTailPageShowsInWindow()) {
            _indicatorWindowIndex++;

            _indicator->runAction(MoveBy::create(_movingDuration,
                        getOneUnitDistance(ScrollDirection::BACKWARD)));
        } else {
            // NOTE: 如果选择的page在view window的头或尾部，滚动会将此page滚动到view window的中间，所以indicator也要移到中间
            _pages->scrollToItem(_indicatorPageIndex, _positionRatioInView, _itemAnchorPoint);

            if (isIndicatorAtTail()) {
                // move indicator to center of view window
                //_indicator->runAction(MoveBy::create(_movingDuration,
                //            getOneUnitDistance(ScrollDirection::FORWARD)));
                _indicator->runAction(MoveTo::create(_movingDuration, _windowCenterPosition));
                //_indicatorWindowIndex--;
                _indicatorWindowIndex = _windowCenterIndex;
            }
        }
    }
    _pages->setCurSelectedIndex(_indicatorPageIndex);

    printf("down >> current page index: %d\n", getCurrentIndex());
    printf("down >> center page index: %d\n", getCenterIndex());
    printf("down >> indicator page index: %d\n", _indicatorPageIndex);
    printf("down >> indicator window index: %d\n", _indicatorWindowIndex);

    return isReverted;
}

int PageViewWindow::getPagesCount() const {
    return _pages->getItems().size();
}

int PageViewWindow::getPagesLastIndex() const {
    return getPagesCount() - 1;
}

int PageViewWindow::getCurrentIndex() const {
    return _pages->getCurSelectedIndex();
}

int PageViewWindow::getHeadIndex() const {
    //return _pages->getIndex(_pages->getTopmostItemInCurrentView());
    return _pages->getIndex(_pages->getCenterItemInCurrentView()) - _numWindowPages / 2;
}

int PageViewWindow::getCenterIndex() const {
    return _pages->getIndex(_pages->getCenterItemInCurrentView());
}

int PageViewWindow::getTailIndex() const {
    return _pages->getIndex(_pages->getBottommostItemInCurrentView());
}

bool PageViewWindow::isIndicatorAtHead() const {
    return _indicatorWindowIndex == 0;
}

bool PageViewWindow::isIndicatorAtTail() const {
    return _indicatorWindowIndex == _numWindowPages - 1;
}

bool PageViewWindow::isIndicatorAtCenter() const {
    return _indicatorWindowIndex == _windowCenterIndex;
}

bool PageViewWindow::isIndicatorBeforeWindowCenter() const {
    return _indicatorWindowIndex < _windowCenterIndex;
}

bool PageViewWindow::isIndicatorBehindWindowCenter() const {
    return _indicatorWindowIndex > _windowCenterIndex;
}

bool PageViewWindow::isPageIndexAtHead() const {
    return _indicatorPageIndex == 0;
}

bool PageViewWindow::isPageIndexAtTail() const {
    return _indicatorPageIndex == getPagesLastIndex();
}

bool PageViewWindow::isHeadPageShowsInWindow() const {
    return getHeadIndex() == 0;
}

bool PageViewWindow::isTailPageShowsInWindow() const {
    return getTailIndex() == getPagesLastIndex();
}
