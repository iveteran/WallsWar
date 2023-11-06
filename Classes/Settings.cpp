#include "Settings.h"
#include "TranslateText.h"

#include "ui/CocosGUI.h"
#include "ui/UILayout.h"

static const char* defaultFont = "fonts/simhei.ttf";
static const int defaultFontSize = 12;
static const float defaultFontScale = 0.5f;
static const Color3B defaultBackgroundColor = Color3B(51, 51, 51); // near gray
static const Color3B defaultActiveColor = Color3B(51, 105, 173); // near blue
static const int defaultOpacity = 128;
static const Size defaultDialogSize(300, 200);

USING_NS_CC;

bool Settings::init() {
    auto size = defaultDialogSize;
    return init(size.width, size.height, defaultBackgroundColor, defaultOpacity);
}

bool Settings::init(int width, int height, const Color3B& bgColor, int bgOpacity) {
    if (!PopupLayer::init(width, height, bgColor, bgOpacity)) {
        return false;
    }

    setTitle(TEXT("settings"));

    int numTabs = 3;
    createSettingsTabs(numTabs);
    createPlayerSettingsPanel(0, "player_settings");
    createServerSettingsPanel(1, "server_settings");
    createScriptsManagementPanel(2, "scripts_management");

    _tabs->setSelectTab(0);
    showEffect();

    return true;
}

void Settings::createPlayerSettingsPanel(int index, const char* title) {
    auto panel = Layout::create();
    auto button = Button::create();
    auto btnLabel = Label::createWithTTF(TEXT("demo"), defaultFont, defaultFontSize);
    btnLabel->setScale(defaultFontScale);
    button->setTitleLabel(btnLabel);
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        auto btn = dynamic_cast<Button*>(sender);
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            btn->getTitleLabel()->setScale(0.5);
            printf("Demo button clicked\n");
            break;
        default:
            break;
        }
    });
    RelativeLayoutParameter* lp = RelativeLayoutParameter::create();
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    button->setLayoutParameter(lp);
    panel->addChild(button);

    addSettingsTab(index, title, panel);
}

void Settings::createServerSettingsPanel(int index, const char* title) {
    auto panel = Layout::create();
    auto button = Button::create();
    auto btnLabel = Label::createWithTTF(TEXT("helloworld"), defaultFont, defaultFontSize);
    btnLabel->setScale(defaultFontScale);
    button->setTitleLabel(btnLabel);
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        auto btn = dynamic_cast<Button*>(sender);
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            btn->getTitleLabel()->setScale(0.5);
            printf("Demo button clicked\n");
            break;
        default:
            break;
        }
    });
    RelativeLayoutParameter* lp = RelativeLayoutParameter::create();
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    button->setLayoutParameter(lp);
    panel->addChild(button);

    addSettingsTab(index, title, panel);
}

void Settings::createScriptsManagementPanel(int index, const char* title) {
    auto panel = Layout::create();
    /*
    auto button = Button::create();
    auto btnLabel = Label::createWithTTF(TEXT("helloworld"), defaultFont, defaultFontSize);
    btnLabel->setScale(defaultFontScale);
    button->setTitleLabel(btnLabel);
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        auto btn = dynamic_cast<Button*>(sender);
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            btn->getTitleLabel()->setScale(0.5);
            printf("Demo button clicked\n");
            break;
        default:
            break;
        }
    });
    RelativeLayoutParameter* lp = RelativeLayoutParameter::create();
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    button->setLayoutParameter(lp);
    panel->addChild(button);
    */
    panel->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    panel->setBackGroundColor(Color3B::BLUE);
    panel->setBackGroundColorOpacity(defaultOpacity);

    addSettingsTab(index, title, panel);
}

void Settings::createSettingsTabs(int numTabs) {
    _tabs = TabControl::create();
    _tabs->setContentSize(Size(getWidth(), getHeight() - getTitleHeight()));
    float headerWidth = getWidth() / numTabs;
    _tabs->setHeaderWidth(headerWidth);
    _tabs->setHeaderHeight(getTitleHeight());
    _tabs->setHeaderActiveBackgroundColor(defaultActiveColor, defaultOpacity);
    _tabs->setHeaderSelectedZoom(0.0f);
    _tabs->setHeaderDockPlace(TabControl::Dock::TOP);

    RelativeLayoutParameter* tabsLP = RelativeLayoutParameter::create();
    tabsLP->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);
    _tabs->setLayoutParameter(tabsLP);
    _tabs->setTabChangedEventListener(
        [&](int index, TabControl::EventType evtType) {
            printf("tab %d selected\n", index);
        }
    );
    addMainPanel(_tabs);
}

void Settings::addSettingsTab(int index, const char* title, Layout* panel) {
    auto header = TabHeader::create();
    auto hdrLabel = Label::createWithTTF(TEXT(title), defaultFont, defaultFontSize);
    hdrLabel->setScale(defaultFontScale);
    hdrLabel->setOpacity(defaultOpacity);
    header->setTitleLabel(hdrLabel);

    panel->setLayoutType(Layout::Type::RELATIVE);
    panel->setContentSize(Size(_tabs->getContentSize().width, _tabs->getContentSize().height - getTitleHeight()));
    //panel->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    //panel->setBackGroundColor(Color3B::GRAY);
    //panel->setBackGroundColorOpacity(100);

    _tabs->insertTab(index, header, panel);
}
