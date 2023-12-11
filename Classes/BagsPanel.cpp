#include "BagsPanel.h"
#include "Player.h"

#include "ui/UIListView.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"

static const int MAX_NUM_BAGS_TO_SHOW = 3;
static const char* defaultFont = "fonts/simhei.ttf";
static int defaultFontSize = 12;
static int largeFontSize = 14;
static const float defaultFontScale = 0.3f;
static const Color3B defaultBackgroundColor = Color3B(51, 51, 51);  // near gray
//static const Color3B defaultBackgroundColor = Color3B(85, 170, 255);  // near blue
static const int defaultOpacity = 128;
static const float defaultIconScale = 0.3f;
static const float defaultBorder = 2;
static const float defaultMargin = 2;

static const int gapFromWeaponWheel = 10;
static const int gapFromBottom = 20;
static const float nameLabelHeight = 5;
static const float countLabelHeight = 5;
static const Size gItemSize(15, 15);
static const Size gBagSize(gItemSize.width, gItemSize.height + countLabelHeight/* + nameLabelHeight*/);
//static const float defaultPanelWidth = 30;
//static const float defaultPanelHeight = 25;

Color3B getBagsPanelColor(ItemCategory category) {
    switch (category) {
        case ItemCategory::PROP:
            return Color3B(100, 100, 100);
            //return Color3B(200, 200, 200);
        case ItemCategory::AD:
            return Color3B(100, 100, 100);
            //return Color3B(200, 200, 200);
        default:
            return Color3B::WHITE;
    }
}

Bag* Bag::create(ItemType type, const char* name, const char* icon) {
    auto *pRet = new(std::nothrow) Bag();
    if (pRet && pRet->init(type, name, icon)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool Bag::init(ItemType type, const char* name, const char* icon) {
    if (!Widget::init()) {
        return false;
    }
    _type = type;
    setContentSize(gBagSize);

    setLayoutType(Layout::Type::VERTICAL);

    auto lp = LinearLayoutParameter::create();
    lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    /*
    auto nameLabel = Text::create(name, defaultFont, defaultFontSize);
    nameLabel->setScale(defaultFontScale);
    nameLabel->setLayoutParameter(lp);
    addChild(nameLabel);
    */

    auto iconBtn = Button::create(icon);
    iconBtn->setScale(defaultIconScale);
    iconBtn->setLayoutParameter(lp->clone());
    addChild(iconBtn);

    iconBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("Bag clicked, type: %d\n", _type);
            break;
        default:
            break;
        }
    });

    auto countStr = StringUtils::format("%d", count());
    _countLabel = Text::create(countStr, defaultFont, defaultFontSize);
    _countLabel->setScale(defaultFontScale);
    _countLabel->setLayoutParameter(lp->clone());
    addChild(_countLabel);

    return true;
}

int Bag::addItem(Item* item) {
    _items.insert(item->id(), item);

    auto countStr = StringUtils::format("%d", count());
    _countLabel->setString(countStr);

    return count();
}

int Bag::dropItem(int itemId) {
    _items.erase(itemId);

    auto countStr = StringUtils::format("%d", count());
    _countLabel->setString(countStr);

    return count();
}

BagsPanel* BagsPanel::create(ItemCategory category) {
    auto *pRet = new(std::nothrow) BagsPanel();
    if (pRet && pRet->init(category)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool BagsPanel::init(ItemCategory category) {
    if (!Layout::init()) {
        return false;
    }
    _category = category;
    setName("bags_panel");
    setContentSize(Size(defaultBorder * 2, gBagSize.height + nameLabelHeight));

    setLayoutType(Layout::Type::VERTICAL);

    auto lp = LinearLayoutParameter::create();
    lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    auto name = getBagCategoryName(_category);
    auto nameLabel = Text::create(name, defaultFont, largeFontSize);
    nameLabel->setScale(defaultFontScale);
    auto lp2 = lp->clone();
    lp2->setMargin(Margin(0, 1, 0, 1));
    nameLabel->setLayoutParameter(lp2);
    addChild(nameLabel);

    _bagsView = ListView::create();
    _bagsView->setContentSize(Size(0, getContentSize().height));
    _bagsView->setDirection(ListView::Direction::HORIZONTAL);
    _bagsView->setBounceEnabled(true);
    _bagsView->setScrollBarEnabled(false);
    _bagsView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    _bagsView->setItemsMargin(defaultMargin);
    _bagsView->forceDoLayout();
    _bagsView->setLayoutParameter(lp->clone());

    _bagsView->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    auto panelColor = getBagsPanelColor(_category);
    _bagsView->setBackGroundColor(panelColor);
    _bagsView->setBackGroundColorOpacity(defaultOpacity);

    addChild(_bagsView);

    return true;
}

void BagsPanel::addBag(ItemType type, const char* name, const char* icon, int index) {
    auto bag = Bag::create(type, name, icon);
    if (index >= 0 ) {
        _bagsView->insertCustomItem(bag, index);
    } else {
        _bagsView->pushBackCustomItem(bag);
        _bagsView->doLayout();

        //float delaySec = count();
        float delaySec = 1.0f;
        _bagsView->scrollToRight(delaySec, false);
        //_bagsView->scrollToLeft(previousIndex, true); // return to previous position
    }
    _bags.insert(type, bag);

    updateWidth(SizeChange::INCREASE);
}

void BagsPanel::removeBag(ItemType type) {
    auto iter = _bags.find(type);
    if (iter == _bags.end()) {
        return;
    }

    auto bag = iter->second;
    _bagsView->removeItem(_bagsView->getIndex(bag));
    _bags.erase(type);

    updateWidth(SizeChange::DECREASE);
}

void BagsPanel::updateWidth(SizeChange direction) {
    if (count() > MAX_NUM_BAGS_TO_SHOW) {
        return;
    }
    float changeSize = gBagSize.width + defaultMargin;
    changeSize *= (int)direction;
    auto selfSize = getContentSize();
    setContentSize(Size(selfSize.width + changeSize, selfSize.height));
    auto viewSize = _bagsView->getContentSize();
    _bagsView->setContentSize(Size(viewSize.width + changeSize, viewSize.height));
}

int BagsPanel::addItem(Item* item) {
    auto type = item->getItemType();
    auto iter = _bags.find(type);
    if (iter == _bags.end()) {
        addBag(type, item->getItemName(), item->getIcon());
    }
    auto bag = _bags.at(type);
    bag->addItem(item);

    _items.insert(item->id(), item);
    return _items.size();
}

int BagsPanel::dropItem(int itemId) {
    auto iter = _items.find(itemId);
    if (iter == _items.end()) {
        return _items.size();
    }
    auto item = iter->second;
    auto type = item->getItemType();
    auto iter2 = _bags.find(type);
    if (iter2 != _bags.end()) {
        iter2->second->dropItem(itemId);
        if (iter2->second->isEmpty()) {
            removeBag(type);
        }
    }
    _items.erase(itemId);
    return _items.size();
}

bool MultiBagsPanel::init() {
    if (!Layout::init()) {
        return false;
    }
    setName("multi_bags_panel");
    setContentSize(Size(0, gBagSize.height + nameLabelHeight));

    setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    setBackGroundColor(defaultBackgroundColor);
    setBackGroundColorOpacity(defaultOpacity);

    setLayoutType(Layout::Type::HORIZONTAL);

    auto lp = RelativeLayoutParameter::create();
    lp->setRelativeName(getName()); //给组件布局属性设置一个名字，别人可以找到它
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL); // 置于屏幕正下方
    //lp->setAlign(RelativeLayoutParameter::RelativeAlign::LOCATION_LEFT_OF_BOTTOMALIGN);
    //lp->setRelativeToWidgetName("weapon_wheel"); //设定当前组件要与哪个组件对齐
    //lp->setMargin(Margin(0, 0, gapFromWeaponWheel, gapFromBottom));
    lp->setMargin(Margin(0, 0, 0, gapFromBottom));
    setLayoutParameter(lp);

    _bagsPanelsView = ListView::create();
    _bagsPanelsView->setContentSize(getContentSize());
    _bagsPanelsView->setDirection(ListView::Direction::HORIZONTAL);
    _bagsPanelsView->setBounceEnabled(true);
    _bagsPanelsView->setScrollBarEnabled(false);
    //_bagsPanelsView->setItemsMargin(defaultMargin);
    _bagsPanelsView->forceDoLayout();
    addChild(_bagsPanelsView);

    return true;
}

void MultiBagsPanel::addBagsPanel(ItemCategory category, int index) {
    auto bagPanel = BagsPanel::create(category);
    if (index >= 0 ) {
        _bagsPanelsView->insertCustomItem(bagPanel, index);
    } else {
        _bagsPanelsView->pushBackCustomItem(bagPanel);
        _bagsPanelsView->doLayout();

        //float delaySec = count();
        float delaySec = 1.0f;
        _bagsPanelsView->scrollToRight(delaySec, false);
        //_bagsPanelsView->scrollToLeft(previousIndex, true); // return to previous position
    }
    _bagsPanels.insert(category, bagPanel);
}

void MultiBagsPanel::removeBagsPanel(ItemCategory category) {
    auto iter = _bagsPanels.find(category);
    if (iter == _bagsPanels.end()) {
        return;
    }
    _bagsPanels.erase(category);

    auto bagsPanel = iter->second;
    _bagsPanelsView->removeItem(_bagsPanelsView->getIndex(bagsPanel));
}

int MultiBagsPanel::addItem(Item* item) {
    auto category = item->getItemCategory();
    auto iter = _bagsPanels.find(category);
    if (iter == _bagsPanels.end()) {
        printf(">> add panel, category: %d\n", category);
        addBagsPanel(category);
    }
    auto bagsPanel = _bagsPanels.at(category);
    bagsPanel->addItem(item);

    _items.insert(item->id(), item);

    updateWidth();
    return _items.size();
}

int MultiBagsPanel::dropItem(int itemId) {
    auto iter = _items.find(itemId);
    if (iter == _items.end()) {
        return _items.size();
    }
    auto item = iter->second;
    auto category = item->getItemCategory();
    auto iter2 = _bagsPanels.find(category);
    if (iter2 != _bagsPanels.end()) {
        iter2->second->dropItem(itemId);
        if (iter2->second->isEmpty()) {
            removeBagsPanel(category);
        }
    }
    _items.erase(itemId);

    updateWidth();
    return _items.size();
}

void MultiBagsPanel::updateWidth() {
    auto selfSize = getContentSize();
    auto viewSize = _bagsPanelsView->getContentSize();
    float width = 0;
    for (auto iter : _bagsPanels) {
        width += iter.second->getContentSize().width;
    }
    setContentSize(Size(width, selfSize.height));
    _bagsPanelsView->setContentSize(Size(width, viewSize.height));
}
