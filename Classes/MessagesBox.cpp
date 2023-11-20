#include "MessagesBox.h"
#include "TranslateText.h"
#include "ToggleButton.h"
#include "PopupMenu.h"
#include "NoticeBar.h"

#include "ui/CocosGUI.h"
#include "ui/UILayout.h"
#include "ui/UIListView.h"
#include "ui/UIText.h"
#include "ui/UITextField.h"

#include "Player.h"
#include "GameScene.h"

static const int defaultWidth = 120;
static const char* defaultFont = "fonts/simhei.ttf";
static const int defaultFontSize = 12;
static const float defaultFontScale = 0.5f;
//static const Color3B defaultBackgroundColor = Color3B(51, 105, 173); // near blue
static const Color3B blueBackgroundColor = Color3B(51, 105, 173); // near blue
static const Color3B defaultBackgroundColor = Color3B(51, 51, 51); // near gray
static const int defaultOpacity = 128;
static const float defaultMargin = 2;
static const float avatarScale = 0.3;
static const int inputFieldHeight = 10;
static const int toolbarFieldHeight = 10;

int64_t generateMessageId();
void parseMessage(const string& rawMsg, string& msgContent, vector<string>& reminderPlayers);

USING_NS_CC;

int64_t generateMessageId() {
    static int64_t _id = 0;
    return _id++;
}

bool MessageToolbar::init() {
    if (!Layout::init()) {
        return false;
    }
    setLayoutType(Layout::Type::VERTICAL);

    setContentSize(Size(defaultWidth, toolbarFieldHeight));

    // NOTE: toolbar在messages box层的外面，所以要单独设置它的背景色和透明度
    setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    setBackGroundColor(defaultBackgroundColor);
    setBackGroundColorOpacity(defaultOpacity);

    float margin = defaultMargin;
    auto lp = LinearLayoutParameter::create();
    lp->setMargin(Margin(margin, margin, margin, margin));

    auto layout = Layout::create();  // create center layout
    layout->setLayoutType(Layout::Type::HORIZONTAL);
    _muteToggle = ToggleButton::create("images/msg-mute.png", "images/msg-unmute.png",
            CC_CALLBACK_1(MessageToolbar::muteMessage, this),
            CC_CALLBACK_1(MessageToolbar::unmuteMessage, this));
    _muteToggle->setScale(defaultFontScale);
    _muteToggle->setLayoutParameter(lp);
    layout->addChild(_muteToggle);

    _cleanupBtn = Button::create("images/msg-clear.png");
    _cleanupBtn->setScale(defaultFontScale);
    _cleanupBtn->setLayoutParameter(lp);
    _cleanupBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            clearMessages();
            break;
        default:
            break;
        }
    });
    layout->addChild(_cleanupBtn);

    auto lp2 = LinearLayoutParameter::create();
    lp2->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout->setLayoutParameter(lp2);

    addChild(layout);

    return true;
}

void MessageToolbar::setMessagesBox(MessagesBox* msgsBox) {
    _msgsBox = msgsBox;
}

void MessageToolbar::muteMessage(const ToggleButton* sender) {
    _msgsBox->mute();
}

void MessageToolbar::unmuteMessage(const ToggleButton* sender) {
    _msgsBox->unmute();
}

void MessageToolbar::clearMessages() {
    _msgsBox->clearMessages();
}

bool MessageInput::init() {
    if (!Layout::init()) {
        return false;
    }
    setLayoutType(Layout::Type::HORIZONTAL);

    setContentSize(Size(defaultWidth, inputFieldHeight));

    float margin = defaultMargin;
    auto lp = LinearLayoutParameter::create();
    lp->setMargin(Margin(margin, margin, margin, margin));

    auto lp2 = LinearLayoutParameter::create();
    lp2->setMargin(Margin(margin, 0, margin, 0));

    _inputTypeBtn = ToggleButton::create("images/msg-keyboard.png", "images/msg-speech.png",
            CC_CALLBACK_1(MessageInput::switchToSpeechInput, this),
            CC_CALLBACK_1(MessageInput::switchToKeyboardInput, this));
    _inputTypeBtn->setScale(defaultFontScale);  // NOTE: 通过增大字体再缩小以提高文件清析度
    _inputTypeBtn->setLayoutParameter(lp);
    addChild(_inputTypeBtn);

    _speechInputBtn = ToggleButton::create("images/msg-speech-on.png", "images/msg-speech-off.png",
            CC_CALLBACK_1(MessageInput::stopSpeech, this),
            CC_CALLBACK_1(MessageInput::startSpeech, this));
    _speechInputBtn->setScale(defaultFontScale);  // NOTE: 通过增大字体再缩小以提高文件清析度
    _speechInputBtn->setLayoutParameter(lp);
    addChild(_speechInputBtn);

    //_kbdInputField = TextField::create("在此输入消息", defaultFont, defaultFontSize);
    //_kbdInputField->setScale(defaultFontScale);
    //_kbdInputField->addEventListener(CC_CALLBACK_2(MessageInput::textFieldEvent, this));
    //_kbdInputField->setContentSize(Size(180, inputFieldHeight));

    Size editBoxSize(defaultWidth, inputFieldHeight);
    _kbdInputField = EditBox::create(editBoxSize, "");
    _kbdInputField->setScale(defaultFontScale);
    _kbdInputField->setMaxLength(80);
    _kbdInputField->setFont(defaultFont, defaultFontSize - 2);
    _kbdInputField->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _kbdInputField->setPlaceHolder("在此输入消息");
    _kbdInputField->setPlaceholderFont(defaultFont, defaultFontSize - 2);
    _kbdInputField->setDelegate(this);
    _kbdInputField->setVisible(true);
    _kbdInputField->setVisible(false);
    _kbdInputField->setLayoutParameter(lp2);
    addChild(_kbdInputField);

    _goBtn = Button::create("images/msg-send.png");
    _goBtn->setScale(defaultFontScale);
    _goBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            sendInputText();
            break;
        default:
            break;
        }
    });
    _goBtn->setVisible(false);
    _goBtn->setLayoutParameter(lp);
    addChild(_goBtn);

    _freqStatementMenu = PopupMenu::create();
    _freqStatementMenu->setLayoutParameter(lp);
    addChild(_freqStatementMenu);

    _freqStatementMenu->addMenuItem("悟空救我", CC_CALLBACK_2(MessageInput::sendText, this));
    _freqStatementMenu->addMenuItem("妖精哪里走", CC_CALLBACK_2(MessageInput::sendText, this));

    _toolbarToggle = ToggleButton::create("images/pull-up.png", "images/pull-down.png",
            CC_CALLBACK_1(MessageInput::hideToolbar, this),
            CC_CALLBACK_1(MessageInput::showToolbar, this));
    _toolbarToggle->setScale(defaultFontScale);  // NOTE: 通过增大字体再缩小以提高文件清析度
    _toolbarToggle->setLayoutParameter(lp);
    addChild(_toolbarToggle);

    return true;
}

void MessageInput::setMessagesBox(MessagesBox* msgsBox) {
    _msgsBox = msgsBox;
}

void MessageInput::switchToKeyboardInput(const ToggleButton* sender) {
    _speechInputBtn->setVisible(false);
    _kbdInputField->setVisible(true);
    _goBtn->setVisible(true);
}

void MessageInput::switchToSpeechInput(const ToggleButton* sender) {
    _kbdInputField->setVisible(false);
    _goBtn->setVisible(false);
    _speechInputBtn->setVisible(true);
}

void MessageInput::stopSpeech(const ToggleButton* sender) {
}

void MessageInput::startSpeech(const ToggleButton* sender) {
}

void MessageInput::sendInputText() {
    const char* msg = _kbdInputField->getText();
    if (msg && strlen(msg) > 0) {
        _msgsBox->addMessage(msg, GET_PLAYER1());
        _kbdInputField->setText("");
    }
}

void MessageInput::sendText(Ref* sender, string& text) {
    if (!text.empty()) {
        _msgsBox->addMessage(text.c_str(), GET_PLAYER1());
    }
}

void MessageInput::editBoxEditingDidBegin(EditBox* editBox)
{
    log("editBox %p DidBegin !", editBox);
}

void MessageInput::editBoxEditingDidEndWithAction(EditBox* editBox, EditBoxDelegate::EditBoxEndAction action)
{
    log("editBox %p DidEnd with action %d!", editBox, action);
}

void MessageInput::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    log("editBox %p TextChanged, text: %s ", editBox, text.c_str());
    //editBox->setText(text.c_str());
}

void MessageInput::editBoxReturn(ui::EditBox* editBox)
{
    log("editBox %p was returned !",editBox);
}

//void MessageInput::textFieldEvent(Ref *pSender, TextField::EventType type)
//{
//    switch (type) {
//        case TextField::EventType::ATTACH_WITH_IME:
//            printf("attach with IME\n");
//            break;
//        case TextField::EventType::DETACH_WITH_IME:
//            printf("detach with IME\n");
//            break;
//        case TextField::EventType::INSERT_TEXT:
//            printf("insert words\n");
//            {
//                auto textField = dynamic_cast<TextField*>(pSender);
//                onTyping(textField->getString());
//            }
//            break;
//        case TextField::EventType::DELETE_BACKWARD:
//            printf("delete word\n");
//            break;
//        default:
//            break;
//    }
//}

void MessageInput::onTyping(const string& text) {
}

void MessageInput::showToolbar(const ToggleButton* sender) {
    _msgsBox->showToolbar();
}

void MessageInput::hideToolbar(const ToggleButton* sender) {
    _msgsBox->hideToolbar();
}

bool MessageBubble::init() {
    if (!Layout::init()) {
        return false;
    }
    setLayoutType(Layout::Type::RELATIVE);

    //setContentSize(Size(100, 15));

    return true;
}

void MessageBubble::buildAvatar(const char* avatarImg) {
    _avatar = Button::create(avatarImg);
    _avatar->setScale(avatarScale);
    auto lp = RelativeLayoutParameter::create();
    lp->setRelativeName("avatar");
    auto alignParam = _msgItem->isOwn ?
        RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT:
        RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT;
    lp->setAlign(alignParam);
    float margin = defaultMargin;
    lp->setMargin(Margin(margin, margin, margin, margin));
    _avatar->setLayoutParameter(lp);
    _avatar->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("> player avatar clicked\n");
            break;
        default:
            break;
        }
    });
    addChild(_avatar);
    printf(">>> avatar size: (%f, %f)\n", _avatar->getContentSize().width, _avatar->getContentSize().height);
}

void MessageBubble::buildContent() {
    string wrappedContent;
    Size textBlockSize = makeStringWrapped(_msgItem->content, wrappedContent);
    printf(">>> textBlockSize: (%f, %f)\n", textBlockSize.width, textBlockSize.height);
    Size bubbleSize = calculateBubbleSize(_avatar->getContentSize(), textBlockSize);
    printf(">>> bubbleSize: (%f, %f)\n", bubbleSize.width, bubbleSize.height);
    setContentSize(bubbleSize);

    _content = Text::create(wrappedContent, defaultFont, defaultFontSize);
    _content->setScale(defaultFontScale);
    _content->setColor(_msgItem->color);
    //_content->setTextAreaSize(textBlockSize);
    //_content->setBackgroundColor(Color3B(51, 105, 173), defaultOpacity);
    //_content->enableShadow();

    auto lp = RelativeLayoutParameter::create();
    lp->setRelativeName("content");
    auto alignParam = _msgItem->isOwn ?
        RelativeLayoutParameter::RelativeAlign::LOCATION_LEFT_OF_TOPALIGN :
        RelativeLayoutParameter::RelativeAlign::LOCATION_RIGHT_OF_TOPALIGN;
    lp->setAlign(alignParam);
    lp->setRelativeToWidgetName("avatar"); //设定当前组件要与哪个组件对齐
    float margin = defaultMargin;
    lp->setMargin(Margin(margin, margin, margin, margin));
    _content->setLayoutParameter(lp);
    addChild(_content);
}

void MessageBubble::setMessageItem(const MessageItem* msgItem) {
    _msgItem = (MessageItem*)msgItem;

    const char* avatarImg = nullptr;
    if (msgItem->playerId == 0) {
        avatarImg = getGameManagerAvatar();
    } else {
        avatarImg = getPlayerAvatar(_msgItem->playerId);
    }
    buildAvatar(avatarImg); // avatar image size: 40x40
    buildContent();
}

const char* MessageBubble::getPlayerAvatar(int64_t playerId) {
    return "images/yuu-avatar.jpg"; // TODO: to get by player id
}

const char* MessageBubble::getGameManagerAvatar() {
    return "images/game-manager.png";
}

Size MessageBubble::makeStringWrapped(const string& content, string& output) {
    float width = 0;
    float avatarHeight = _avatar->getContentSize().height;
    float height = content.size() > 25 ? 30 : 15;
    output = content;
    return Size(width, height);
}

Size MessageBubble::calculateBubbleSize(Size avatarSize, Size textBlockSize) {
    avatarSize = avatarSize * avatarScale;
    float width = avatarSize.width + textBlockSize.width + defaultMargin * 4;
    float height = std::max(avatarSize.height, textBlockSize.height);
    return Size(width, height);
}

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
    setName("messages_box");

    auto lp = RelativeLayoutParameter::create();
    lp->setRelativeName(getName()); //给组件布局属性设置一个名字，别人可以找到它
    // 对齐status bar右下角
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::LOCATION_BELOW_RIGHTALIGN);
    lp->setRelativeToWidgetName("status_bar"); //设定当前组件要与哪个组件对齐
    setLayoutParameter(lp);

    setTitle(TEXT("Messages"));

    // 两层layout, 第一层用于放置每二层和NoticeBar, 第二层用于放置msg, input等
    // NoticeBar放在顶部，在第二层之上
    auto widgetSize = getMainPanelSize();
    _layout = Layout::create(); // 第一层
    _layout->setLayoutType(Layout::Type::RELATIVE);
    _layout->setContentSize(widgetSize);

    auto layout = Layout::create(); // 第二层
    layout->setLayoutType(Layout::Type::VERTICAL);
    layout->setContentSize(widgetSize);
    _layout->addChild(layout);

    _showMsgList = ListView::create();
    _showMsgList->setDirection(ui::ScrollView::Direction::VERTICAL);
    _showMsgList->setBounceEnabled(true);
    _showMsgList->setContentSize(Size(widgetSize.width, widgetSize.height - inputFieldHeight/* - toolbarFieldHeight*/));
    _showMsgList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _showMsgList->setScrollBarWidth(2);
    _showMsgList->setGravity(ListView::Gravity::CENTER_VERTICAL);

    float spacing = 4;
    _showMsgList->setItemsMargin(spacing);
    _showMsgList->forceDoLayout();
    //float totalHeight = _itemTemplateHeight * _totalCount + (_totalCount - 1) * spacing;
    //_showMsgList->setInnerContainerSize(Size(_showMsgList->getInnerContainerSize().width, totalHeight));
    layout->addChild(_showMsgList);

    generateDemoMessages(10);

    //_showMsgList->jumpToBottom();

    _msgInput = MessageInput::create();
    _msgInput->setMessagesBox(this);
    layout->addChild(_msgInput);

    _toolbar = MessageToolbar::create();
    _toolbar->setMessagesBox(this);
    _toolbar->setVisible(false);
    layout->addChild(_toolbar);

    addMainPanel(_layout);

    showEffect();

    scheduleUpdate();

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
            value = Size(defaultWidth, 60);
            break;
    }
    return value;
}

void MessagesBox::changeSize(BoxSize sizeType) {
    auto size = getCustomizedSize(sizeType);
    setContentSize(size);
}

void MessagesBox::mute() {
    _isMute = true;
}

void MessagesBox::unmute() {
    _isMute = false;
}

void MessagesBox::close() {
    setVisible(false);
}

void MessagesBox::showToolbar() {
    _toolbar->setVisible(true);
    //auto mbSize = getContentSize();
    //setContentSize(Size(mbSize.width, mbSize.height + toolbarFieldHeight));
}

void MessagesBox::hideToolbar() {
    _toolbar->setVisible(false);
    //auto mbSize = getContentSize();
    //setContentSize(Size(mbSize.width, mbSize.height - toolbarFieldHeight));
}

void MessagesBox::enableAutoPlayVoice(bool enable) {
}

void MessagesBox::playUnreadVoiceMessaes() {
}

void MessagesBox::enableAutoConvertVoiceToText(bool enable) {
}

void MessagesBox::convertUnreadVoicesToText() {
}

void MessagesBox::addMessage(const string& message, const Player* player) {
    auto msgItem = MessageItem::create();
    msgItem->id = generateMessageId();
    msgItem->time = time(NULL);
    if (player) {
        msgItem->playerId = player->id();
        msgItem->playerName = player->getName();
        if (GET_PLAYER1()) {
            msgItem->isOwn = player->id() == GET_PLAYER1()->id();
        }
    }
    msgItem->color = Color3B::WHITE;

    parseMessage(message, msgItem->content, msgItem->reminderPlayers);

    auto msgBubble = MessageBubble::create();
    msgBubble->setMessageItem(msgItem);

    if (msgItem->isOwn) {
        auto lp = LinearLayoutParameter::create();
        lp->setGravity(LinearLayoutParameter::LinearGravity::RIGHT);
        msgBubble->setLayoutParameter(lp);
    }

    _showMsgList->pushBackCustomItem(msgBubble);
    _showMsgList->jumpToBottom();
}

void MessagesBox::addSystemMessage(const string& message, NoticeLevel level)  {
    auto msgItem = MessageItem::create();
    msgItem->id = generateMessageId();
    msgItem->content = message;
    msgItem->color = getMessageColor(level);
    msgItem->level = level;
    msgItem->time = time(NULL);

    auto msgBubble = MessageBubble::create();
    msgBubble->setContentSize(Size(getContentSize().width, 0)); // the height will be set by message lines late
    msgBubble->setMessageItem(msgItem);

    _showMsgList->pushBackCustomItem(msgBubble);
}

int MessagesBox::addNotice(const string& message, NoticeLevel level) {
    if (!_noticeBar) {
        _noticeBar = NoticeBar::create();
        _noticeBar->setRemovedCallback(CC_CALLBACK_1(MessagesBox::onNoticeRemoved, this));

        auto lp = RelativeLayoutParameter::create();
        lp->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
        _noticeBar->setLayoutParameter(lp);

        _layout->addChild(_noticeBar);
    }
    return _noticeBar->addNotice(message, level);
}

void MessagesBox::removeNotice(int id) {
    if (_noticeBar) {
        _noticeBar->removeNotice(id);
    }
}

void MessagesBox::onNoticeRemoved(const NoticeItem* noticeItem) {
    // NOTE: NoticeItem will be nullptr
    if (_noticeBar->empty()) {
        removeChild(_noticeBar);
        _noticeBar = nullptr;
    }
}

void MessagesBox::loadHistory(time_t since, int count) {
}

void MessagesBox::clearMessages() {
}

void MessagesBox::generateDemoMessages(int count) {
    addSystemMessage("hello world", NoticeLevel::INFO);
    addSystemMessage("你好，世界!", NoticeLevel::WARNING);

    auto player = Player::create();
    addMessage("hello GOE", player);

    auto player1 = GET_PLAYER1();
    addMessage("hello world, hello world, \nhello world, hello world, \nhello world", player1);
}

void parseMessage(const string& rawMsg, string& msgContent, vector<string>& reminderPlayers) {
    bool capturing = false;
    const char mark = '@';
    string markStr;
    markStr = mark;
    const char space = ' ';

    for (char ch : rawMsg) {
        if (ch == mark) {
            capturing = true;
            reminderPlayers.push_back("");
        } else if (ch == space) {
            if (capturing && !reminderPlayers.empty() &&
                    *(reminderPlayers.end()) == markStr) {
                // 如果只有mark一个字符就删除它
                reminderPlayers.pop_back();
            }
            capturing = false;
        }
        if (capturing) {
            (*reminderPlayers.end()).push_back(ch);
        } else {
            if (ch == space && msgContent.back() == space) {
                // 如是连续空格只保留一个
                continue;
            }
            msgContent.push_back(ch);
        }
    }
}
