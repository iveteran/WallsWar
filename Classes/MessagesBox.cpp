#include "MessagesBox.h"
#include "TranslateText.h"

#include "ui/CocosGUI.h"
#include "ui/UILayout.h"

static const char* defaultFont = "fonts/simhei.ttf";
static const int defaultFontSize = 12;
static const float defaultFontScale = 0.5f;
//static const Color3B defaultBackgroundColor = Color3B(51, 105, 173); // near blue
static const Color3B defaultBackgroundColor = Color3B(51, 51, 51); // near gray
static const int defaultOpacity = 128;

USING_NS_CC;

bool MessagesBox::init() {
    return init(BoxSize::NORMAL, defaultBackgroundColor, defaultOpacity);
}

bool MessagesBox::init(BoxSize sizeType, const Color3B& bgColor, int bgOpacity) {
    auto size = getCustomizedSize(sizeType);
    return init(size.width, size.height, bgColor, bgOpacity);
}

bool MessagesBox::init(int width, int height, const Color3B& bgColor, int bgOpacity) {
    if (!PopupLayer::init(width, height, bgColor, bgOpacity)) {
        return false;
    }

    auto lp = RelativeLayoutParameter::create();
    lp->setRelativeName("messages_box"); //给组件布局属性设置一个名字，别人可以找到它
    // 对齐status bar右下角
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::LOCATION_BELOW_RIGHTALIGN);
    lp->setRelativeToWidgetName("status_bar"); //设定当前组件要与哪个组件对齐
    setLayoutParameter(lp);

    setTitle(TEXT("Messages"));

    showEffect();

    return true;
}

Size MessagesBox::getCustomizedSize(BoxSize sizeType) {
    Size value;
    switch (sizeType) {
        case BoxSize::SMALL:
            value = Size(80, 40);
            break;
        case BoxSize::LARGE:
            value = Size(200, 100);
            break;
        case BoxSize::HUGE:
            value = Size(400, 200);
            break;
        case BoxSize::NORMAL:
        default:
            value = Size(120, 60);
            break;
    }
    return value;
}

void MessagesBox::changeSize(BoxSize sizeType) {
    auto size = getCustomizedSize(sizeType);
    setContentSize(size);
}

void MessagesBox::turnOn() {
}

void MessagesBox::turnOff() {
}

void MessagesBox::close() {
}

void MessagesBox::expandSettingsPanel() {
}

void MessagesBox::enableAutoPlayVoice(bool enable) {
}

void MessagesBox::playUnreadVoiceMessaes() {
}

void MessagesBox::enableAutoConvertVoiceToText(bool enable) {
}

void MessagesBox::convertUnreadVoicesToText() {
}

void MessagesBox::addMessage(const Player* player, string& message) {
}

void MessagesBox::addSystemMessage(NoticeType type, string& message)  {
}

void MessagesBox::loadHistory(time_t since, int count) {
}
