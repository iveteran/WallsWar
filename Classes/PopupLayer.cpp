#include "PopupLayer.h"

#include "ui/CocosGUI.h"
#include "ui/UILayout.h"

static const char* defaultFont = "fonts/simhei.ttf";
static const int defaultOpacity = 128;
static const int defaultTitleHeight = 10;

USING_NS_CC;

bool PopupLayer::init(int width, int height, const Color3B& bgColor, int opacity, const char* bgImage) {
    if (!Layout::init()) {
        return false;
    }
    setContentSize(Size(width, height));

    return _init(width, height, bgColor, opacity, bgImage);
}

bool PopupLayer::initWithModal(int width, int height, const Color3B& bgColor, int opacity, const char* bgImage) {
    if (!Layout::init()) {
        return false;
    }

    // 设置模态背景颜色和透明度
    setBackGroundColor(Color3B(0, 0, 0));
    setBackGroundColorOpacity(opacity);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    setContentSize(visibleSize);   // 设置模态背景大小：窗口大小

    setSwallowTouches();

    return _init(width, height, bgColor, opacity, bgImage);
}

bool PopupLayer::_init(int width, int height, const Color3B& bgColor, int opacity, const char* bgImage) {
    _font = defaultFont;

    _layout = Layout::create();
    _layout->setContentSize(Size(width, height));
    _layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _layout->setBackGroundColor(bgColor);
    _layout->setBackGroundColorOpacity(opacity);
    _layout->setPosition((getContentSize() - _layout->getContentSize()) / 2); // 居中
    _layout->setLayoutType(Layout::Type::RELATIVE);
    addChild(_layout);

    // 创建Title栏
    auto titleLayout = Layout::create();
    titleLayout->setContentSize(Size(_layout->getContentSize().width, defaultTitleHeight));
    titleLayout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    titleLayout->setBackGroundColorOpacity(defaultOpacity);
    titleLayout->setBackGroundColor(_titleBgColor);
    titleLayout->setLayoutType(Layout::Type::RELATIVE);

    RelativeLayoutParameter* titleLP = RelativeLayoutParameter::create();
    titleLP->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    titleLayout->setLayoutParameter(titleLP);
    _layout->addChild(titleLayout);

    // 创建Title text
    _titleLabel = Text::create("No Title", _font, _fontSize);
    _titleLabel->setScale(_fontScale);  // NOTE: 通过增大字体再缩小以提高文件清析度
    RelativeLayoutParameter* lp2 = RelativeLayoutParameter::create();
    lp2->setAlign(RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    _titleLabel->setLayoutParameter(lp2);
    titleLayout->addChild(_titleLabel);

    // 创建关闭按钮，位于右上角
    auto button = Button::create("images/close-white-2.png");
    //button->setTitleText("X");
    button->setScale(_fontScale);
    RelativeLayoutParameter* btnLP = RelativeLayoutParameter::create();
    btnLP->setMargin(Margin(0, 0, 2, 0)); // 右边"留白"2像素
    btnLP->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    button->setLayoutParameter(btnLP);
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            _closedCallback();
            this->removeFromParentAndCleanup(true);
            break;
        default:
            break;
        }
    });
    titleLayout->addChild(button);

    return true;
}

void PopupLayer::setClosedCallback(const dialogClosedCallback& callback) {
    _closedCallback = callback;
}

void PopupLayer::setFont(const char* font, int size, float scale) {
    _font = font;
    _fontSize = size;
    _fontScale = scale;
}

void PopupLayer::setTitleBackgroundColor(const Color3B& color) {
    _titleBgColor = color;
}

void PopupLayer::setTitle(const char* title) {
    _titleLabel->setString(title);
}

int PopupLayer::getTitleHeight() const {
    return defaultTitleHeight;
}

float PopupLayer::getWidth() const {
    return _layout ? _layout->getContentSize().width : 0.0f;
}

float PopupLayer::getHeight() const {
    return _layout ? _layout->getContentSize().height : 0.0f;
}

void PopupLayer::addMainPanel(Widget* widget) {
    auto lp = RelativeLayoutParameter::create();
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);
    widget->setLayoutParameter(lp);

    _layout->addChild(widget);
}

// 弹出特效
void PopupLayer::showEffect() {
    Sequence *popupActions = Sequence::create(
            ScaleTo::create(0.0, 0.0), 
            ScaleTo::create(0.06, 1.05),
            ScaleTo::create(0.08, 0.95),
            ScaleTo::create(0.08, 1.0), NULL);
    _layout->runAction(popupActions);
}

// 重写触摸事件处理函数，达到“模态”效果
void PopupLayer::setSwallowTouches() {
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

// 返回true，屏蔽其它层，达到“模态”效果
bool PopupLayer::onTouchBegan(Touch *touch, Event *event) {
    return true;
}
