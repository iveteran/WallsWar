#include "TeammatesPanel.h"
#include "Player.h"

#include "ui/UIListView.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"

static const int MAX_NUM_TEAMMATES_TO_SHOW = 5;
static const char* defaultFont = "fonts/simhei.ttf";
static int defaultFontSize = 10;
static const Color3B defaultBackgroundColor = Color3B(51, 51, 51);  // near gray
//static const Color3B defaultBackgroundColor = Color3B(85, 170, 255);  // near blue
static const int defaultOpacity = 128;
static const float defaultIconScale = 0.5f;

static const int gapFromZoomOutMap = 10;
static const int gapFromStatusBar = 3;
static const Size gTeammateSize(15, 15);
static const float defaultPanelWidth = 30;
static const float defaultPanelHeight = 20;
static const float countLabelHeight = 5;

Teammate* Teammate::create(Player* player) {
    auto *pRet = new(std::nothrow) Teammate();
    if (pRet && pRet->init(player)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool Teammate::init(Player* player) {
    if (!Widget::init()) {
        return false;
    }
    _player = player;
    setContentSize(gTeammateSize);

    /* 问题：1) 用clipping裁减为圆型头象会有锯齿，2) 无法点击关像进行拖动（没有处理ListView的拖动事件）
    *
    // 抗锯齿问题，在glDrawArrays(GL_TRIANGLES, 0, count);语句之后添加glDisable(GL_POLYGON_SMOOTH);
    auto stencil = Sprite::create("images/avatar-circle-mask.png");
    auto clip = ClippingNode::create();
    clip->setStencil(stencil);
    clip->setInverted(false);
    clip->setAlphaThreshold(0.0f);
    clip->setScale(0.4);
    clip->setPosition(getContentSize() / 2);
    addChild(clip);
    */

    auto avatarBtn = Button::create(player->getAvatar());
    avatarBtn->setScale(0.3);
    avatarBtn->setPosition(getContentSize() / 2);
    avatarBtn->setTag(_player->id());
    //clip->addChild(avatarBtn);
    addChild(avatarBtn);

    avatarBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        auto btn = dynamic_cast<Button*>(sender);
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("avatarBtn clicked, player id: %d\n", btn->getTag());
            break;
        default:
            break;
        }
    });

    return true;
}

bool TeammatesPanel::initWithPlayer(Player* player) {
    if (!Layout::init()) {
        return false;
    }
    setName("teammates");
    setContentSize(Size(defaultPanelWidth, defaultPanelHeight));

    setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    setBackGroundColor(defaultBackgroundColor);
    setBackGroundColorOpacity(defaultOpacity);

    setLayoutType(Layout::Type::RELATIVE);

    auto lp = RelativeLayoutParameter::create();
    lp->setRelativeName(getName()); //给组件布局属性设置一个名字，别人可以找到它
    // 对齐status bar中间下方
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::LOCATION_RIGHT_OF_TOPALIGN);
    lp->setRelativeToWidgetName("zoom_out_map"); //设定当前组件要与哪个组件对齐
    lp->setMargin(Margin(gapFromZoomOutMap, gapFromStatusBar, 0, 0));
    setLayoutParameter(lp);

    addCountLabel();
    addSelf(player);

    _teammateViews = ListView::create();
    _teammateViews->setContentSize(Size(getContentSize().width, getContentSize().height - countLabelHeight));
    _teammateViews->setDirection(ListView::Direction::HORIZONTAL);
    _teammateViews->setBounceEnabled(true);
    _teammateViews->setGravity(ListView::Gravity::BOTTOM);
    _teammateViews->setScrollBarEnabled(false);
    _teammateViews->forceDoLayout();
    auto lp2 = RelativeLayoutParameter::create();
    lp2->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
    _teammateViews->setLayoutParameter(lp2);
    addChild(_teammateViews);

    return true;
}

void TeammatesPanel::addCountLabel() {
    _countLabel = Text::create("0", defaultFont, defaultFontSize);
    _countLabel->setScale(defaultIconScale);
    auto lp = RelativeLayoutParameter::create();
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    _countLabel->setLayoutParameter(lp);
    addChild(_countLabel);
}

void TeammatesPanel::addSelf(Player* player) {
    _self = Teammate::create(player);
    _self->setType(TeammateType::SELF);
    auto lp = RelativeLayoutParameter::create();
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    _self->setLayoutParameter(lp);
    addChild(_self);
}

void TeammatesPanel::addTeammate(Player* player, int index) {
    auto teammate = Teammate::create(player);
    teammate->setType(TeammateType::NORMAL);
    if (index >= 0 ) {
        _teammateViews->insertCustomItem(teammate, index);
    } else {
        _teammateViews->pushBackCustomItem(teammate);
        //float delaySec = count();
        float delaySec = 1.0f;
        _teammateViews->scrollToRight(delaySec, false);
        //_teammateViews->scrollToLeft(previousIndex, true); // return to previous position
    }
    _teammates.insert(player->id(), teammate);

    updateWidthAndCountLabel();
}

void TeammatesPanel::removeTeammate(int64_t playerId) {
    auto iter = _teammates.find(playerId);
    if (iter == _teammates.end()) {
        return;
    }

    auto teammate = iter->second;
    _teammateViews->removeItem(_teammateViews->getIndex(teammate));
    _teammates.erase(playerId);

    updateWidthAndCountLabel();
}

void TeammatesPanel::updateWidthAndCountLabel() {
    if (count() <= MAX_NUM_TEAMMATES_TO_SHOW) {
        updateWidth();
    }
    _countLabel->setString(StringUtils::format("%d", count()));
}

void TeammatesPanel::updateWidth() {
    auto size = Size(gTeammateSize.width * count(), getContentSize().height);
    _teammateViews->setContentSize(size);
    setContentSize(Size(size.width + gTeammateSize.width, getContentSize().height));
}

int TeammatesPanel::count() const {
    return _teammateViews ? _teammateViews->getItems().size() : 0;
}

void TeammatesPanel::voteLeader(int64_t playerId) {
}
