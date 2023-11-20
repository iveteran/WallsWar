#include "PopupMenu.h"

#include "ui/CocosGUI.h"

static const char* defaultFont = "fonts/simhei.ttf";
static const int defaultFontSize = 10;
static const float defaultFontScale = 0.5;
static const Color3B defaultBackgroundColor = Color3B(51, 51, 51); // near gray
static const int defaultOpacity = 128;
static const Size mainButtonSize(40, 40);

USING_NS_CC;

bool PopupMenuItem::init() {
    if (!Button::init()) {
        return false;
    }
    setTitleFontName(defaultFont);
    setTitleFontSize(defaultFontSize);
    setTitleAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    setScale(defaultFontScale);

    addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            if (_callback) {
                string title = getTitleText();
                _callback(this, title);
            }
            break;
        default:
            break;
        }
    });
    return true;
}

void PopupMenuItem::setClickedCallback(const clickMenuItemCallback callback) {
    _callback = callback;
}

bool PopupMenuPanel::init() {
    bool enableTitle = false;
    if (!PopupLayer::init(30, 30, defaultBackgroundColor, defaultOpacity, enableTitle)) {
        return false;
    }
    auto panelSize = getContentSize();
    float margin = 6;

    _menuItems = ListView::create();
    _menuItems->setDirection(ui::ScrollView::Direction::VERTICAL);
    _menuItems->setBounceEnabled(true);
    _menuItems->setContentSize(Size(panelSize.width - margin, panelSize.height - margin));
    _menuItems->setScrollBarEnabled(false);
    _menuItems->setGravity(ListView::Gravity::CENTER_VERTICAL);

    float spacing = 4;
    _menuItems->setItemsMargin(spacing);
    _menuItems->forceDoLayout();

    addMainPanel(_menuItems, margin / 2);

    return true;
}

void PopupMenuPanel::addMenuItem(const char* name, const clickMenuItemCallback& clickedCallback) {
    auto menuItem = PopupMenuItem::create();
    menuItem->setTitleText(name);
    menuItem->setClickedCallback(clickedCallback);

    _menuItems->pushBackCustomItem(menuItem);
}

bool PopupMenu::init() {
    if (!Layout::init()) {
        return false;
    }

    _toggleBtn = Button::create();
    _toggleBtn->loadTextureNormal("images/menu-btn.png");
    _toggleBtn->setScale(defaultFontScale);
    _toggleBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            toggleMenuItems();
            break;
        default:
            break;
        }
    });
    addChild(_toggleBtn);

    setContentSize(_toggleBtn->getContentSize() * defaultFontScale);
    setLayoutType(Layout::Type::RELATIVE);

    auto lp = RelativeLayoutParameter::create();
    lp->setRelativeName("popup_menu_btn");
    setLayoutParameter(lp);

    _popupMenuPanel = PopupMenuPanel::create();
    _popupMenuPanel->addMenuItem("Demo #1", CC_CALLBACK_1(PopupMenu::demoMenuItemClicked, this));
    _popupMenuPanel->setVisible(false);
    _popupMenuPanel->setAnchorPoint(Point(1, 1));

    auto lp2 = RelativeLayoutParameter::create();
    lp2->setRelativeName("popup_menu");
    // 对齐popup_menu_btn右下角
    lp2->setAlign(RelativeLayoutParameter::RelativeAlign::LOCATION_BELOW_RIGHTALIGN);
    lp2->setRelativeToWidgetName("popup_menu_btn"); //设定当前组件要与哪个组件对齐
    _popupMenuPanel->setLayoutParameter(lp2);
    addChild(_popupMenuPanel);

    return true;
}

void PopupMenu::demoMenuItemClicked(PopupMenuItem* menuItem) {
    printf("PopupMenu::demoMenuItemClicked called\n");
}

void PopupMenu::addMenuItem(const char* name, const clickMenuItemCallback& clickedCallback) {
    _popupMenuPanel->addMenuItem(name, clickedCallback);
}

void PopupMenu::toggleMenuItems() {
    _popupMenuPanel->setVisible(!_popupMenuPanel->isVisible());
}
