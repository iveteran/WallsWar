#include "NoticeBar.h"

#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "ui/UIPageView.h"
#include "ui/UIImageView.h"
#include "ccUTF8.h"

USING_NS_CC;

static const float STATS_INTERVAL_FEW_SECONDS = 3;  // seconds
static const int defaultWidth = 120;
static const char* defaultFont = "fonts/simhei.ttf";
static const int defaultFontSize = 12;
static const float defaultFontScale = 0.5f;
static const Color3B defaultBackgroundColor = Color3B(51, 51, 51); // near gray
static const Margin defaultMargin = Margin(3, 0, 3, 0);
static const int defaultOpacity = 200;
static const int noticeBarHeight = 10;

int64_t generateNoticeId() {
    static int64_t _id = 0;
    return _id++;
}

const char* getNoticeIconByLevel(NoticeLevel level) {
    const char *result = nullptr;
    switch (level) {
        case NoticeLevel::INFO:
            result = "msg-level-info.png";
            break;
        case NoticeLevel::NOTICE:
            result = "msg-level-notice.png";
            break;
        case NoticeLevel::WARNING:
            result = "msg-level-warning.png";
            break;
        case NoticeLevel::ERROR:
            result = "msg-level-error.png";
            break;
        default:
            break;
    }
    return result;
}

Color3B getMessageColor(NoticeLevel level) {
    Color3B value;
    switch (level) {
        case NoticeLevel::ERROR:
            value = Color3B::RED;
            break;
        case NoticeLevel::WARNING:
            value = Color3B::YELLOW;
            break;
        default:
            value = Color3B::WHITE;
            break;
    }
    return value;
}

NoticeItem*
NoticeItem::create(NoticeLevel level, const string& msg,
        const removeNoticeCallback& removingCb) {
    auto *pRet = new(std::nothrow) NoticeItem();
    if (pRet && pRet->init(level, msg, removingCb)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool NoticeItem::init(NoticeLevel level, const string& msg,
        const removeNoticeCallback& removingCb) {
    if (!Layout::init()) {
        return false;
    }
    setLayoutType(Layout::Type::RELATIVE);

    _id = generateNoticeId();
    _level = level;
    _removingCb = removingCb;

    auto bgColor = getMessageColor(level);
    setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    setBackGroundColor(defaultBackgroundColor);
    setBackGroundColorOpacity(defaultOpacity);

    const char* imageName = getNoticeIconByLevel(level);
    if (imageName) {
        _levelIcon = ImageView::create(StringUtils::format("images/%s", imageName));
        _levelIcon->setScale(defaultFontScale);
        auto lp = RelativeLayoutParameter::create();
        lp->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
        lp->setMargin(defaultMargin);
        _levelIcon->setLayoutParameter(lp);
        addChild(_levelIcon);
    }

    _message = Text::create(msg, defaultFont, defaultFontSize);
    _message->setScale(defaultFontScale);
    auto lp = RelativeLayoutParameter::create();
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    _message->setLayoutParameter(lp);
    addChild(_message);

    _removingBtn = Button::create("images/close-white-cycle.png");
    _removingBtn->setScale(defaultFontScale);
    auto lp2 = RelativeLayoutParameter::create();
    lp2->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    lp2->setMargin(defaultMargin);
    _removingBtn->setLayoutParameter(lp2);
    _removingBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            _removingCb(this);
            break;
        default:
            break;
        }
    });
    addChild(_removingBtn);

    return true;
}

bool NoticeBar::init() {
    if (!Layout::init()) {
        return false;
    }
    setName("notice_bar");
    setContentSize(Size(defaultWidth, noticeBarHeight));

    auto lp = RelativeLayoutParameter::create();
    lp->setRelativeName(getName()); // 给组件布局属性设置一个名字，别人可以找到它
    // 对齐status bar右下角
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::LOCATION_BELOW_RIGHTALIGN);
    lp->setRelativeToWidgetName("status_bar"); // 设定当前组件要与哪个组件对齐
    setLayoutParameter(lp);

    _container = PageView::create();
    _container->setDirection(PageView::Direction::VERTICAL);
    _container->setContentSize(getContentSize());
    addChild(_container);

    // 默认渲染更新
    scheduleUpdate();

    return true;
}

void NoticeBar::setRemovedCallback(const removeNoticeCallback& cb) {
    _noticeRemovedCb = cb;
}

int NoticeBar::addNotice(const string& msg, NoticeLevel level) {
    auto noticeItem = NoticeItem::create(level, msg, CC_CALLBACK_1(NoticeBar::onRemovingClicked, this));
    _container->addPage(noticeItem);
    _container->jumpToBottom();
    return noticeItem->getId();
}

void NoticeBar::removeNotice(int id) {
    // TODO
}

void NoticeBar::onRemovingClicked(NoticeItem* noticeItem) {
    printf(">> onRemovingClicked called\n");
    _container->removePage(noticeItem);
    if (_noticeRemovedCb) {
       _noticeRemovedCb(nullptr);
    }
}


bool NoticeBar::empty() const {
    return _container->getItems().empty();
}

int NoticeBar::getSize() const {
    return _container->getItems().size();
}

void NoticeBar::rollNotice() {
    int total = getSize();
    if (total < 2) {
        return;
    }
    _currIndex = (_currIndex + 1) % total;
    _container->scrollToItem(_currIndex);
}

void NoticeBar::update(float dt) {
    _accumDt += dt;

    if (_accumDt > STATS_INTERVAL_FEW_SECONDS) {
        rollNotice();
        _accumDt = 0;
    }
}
