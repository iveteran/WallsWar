#ifndef _PAGE_VIEW_WINDOW_H_
#define _PAGE_VIEW_WINDOW_H_

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIListView.h"

USING_NS_CC;

namespace cocos2d::ui {
    class ListView;
    class ImageView;
}
using namespace cocos2d::ui;

class PageViewItem : public Widget {
friend class PageViewWindow;

public:
    using ClickedCallback = std::function<void(PageViewItem*)>;
    virtual bool init() override;

protected:
    ClickedCallback _clickedCallback;
};

class PageViewWindow : public Layout {
public:
    enum WindowPosition {
        HEAD,
        CENTER,
        TAIL,
    };
    enum Direction {
        NONE,
        VERTICAL,
        HORIZONTAL,
    };
    enum ScrollDirection {
        BACKWARD = -1,
        FORWARD = 1,
    };

    CREATE_FUNC(PageViewWindow);
    virtual bool init() override { return true; }
    bool init(Direction direction, Size pageSize, Size indicatorSize,
        int numWindowPages=3, int indicatorWindowIndex=1);

    void setWindowPages(int pages);
    void pushBackPage(PageViewItem* page);

    bool scrollForward();
    bool scrollBackward();

    int getCurrentIndex() const;

protected:
    void handleScrollEvent(Ref* sender, ListView::EventType event);

    Vec2 calculateIndicatorPosition(int indicatorIndex);
    void adjustCurrentPageIndex();
    void onPageItemClicked(PageViewItem* page);
    Vec2 getMultipleUnitsDistance(int numUnits);
    Vec2 getOneUnitDistance(ScrollDirection directionFlag);
    Vec2 getFullWindowDistance(ScrollDirection directionFlag);

    int getPagesCount() const;
    int getPagesLastIndex() const;
    int getHeadIndex() const;
    int getCenterIndex() const;
    int getTailIndex() const;
    bool isIndicatorAtHead() const;
    bool isIndicatorAtTail() const;
    bool isIndicatorAtCenter() const;
    bool isPageIndexAtHead() const;
    bool isPageIndexAtTail() const;
    bool isIndicatorBeforeWindowCenter() const;
    bool isIndicatorBehindWindowCenter() const;
    bool isHeadPageShowsInWindow() const;
    bool isTailPageShowsInWindow() const;

private:
    ListView* _pages = nullptr;
    ImageView* _indicator = nullptr;
    bool _reverseMode;
    WindowPosition _windowInitialPosition;
    int _numWindowPages = 0;
    int _direction = Direction::NONE;
    Size _pageSize;
    Size _indicatorSize;

    int _indicatorPageIndex = 0;
    int _indicatorWindowIndex = 0;
    int _windowCenterIndex = 0;
    Vec2 _windowCenterPosition;

    Vec2 _positionRatioInView;
    Vec2 _itemAnchorPoint;
    float _movingDuration = 0.5f;
};

#endif  // _PAGE_VIEW_WINDOW_H_
