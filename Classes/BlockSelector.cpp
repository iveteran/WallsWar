#include "BlockSelector.h"
#include "PageViewWindow.h"
#include "StackedButton.h"
#include "Camp.h"
#include "Player.h"

#include "ui/UIButton.h"

enum BlockMultiplesIndex {
    BLOCK_4X,
    BLOCK_2X,
    BLOCK_1X,
};

static const char* defaultFont = "fonts/simhei.ttf";
static int defaultFontSize = 10;
static const Color3B defaultBackgroundColor = Color3B(51, 51, 51);  // near gray
static const Color3B blueBackgroundColor = Color3B(85, 170, 255);  // near blue
static const int defaultOpacity = 128;
static const float defaultIconScale = 0.3f;

static const int MAX_NUM_BLOCKS_TO_SHOW = 3;
static const int gapFromWindowRight = 20;
static const Size gBlockButtonSize(15, 15);
static const Size blockIndicatorSize(5, 5);
static const float multiplyButtonHeight = gBlockButtonSize.height;
static const float defaultPanelWidth = gBlockButtonSize.width + blockIndicatorSize.width;
static const float defaultPanelHeight = gBlockButtonSize.height * MAX_NUM_BLOCKS_TO_SHOW + multiplyButtonHeight;

BlockWrapper* BlockWrapper::create(BlockType type, const char* icon) {
    auto *pRet = new(std::nothrow) BlockWrapper();
    if (pRet && pRet->init(type, icon)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool BlockWrapper::init(BlockType type, const char* icon) {
    if (!PageViewItem::init()) {
        return false;
    }
    //setTouchEnabled(true);
    setContentSize(gBlockButtonSize);
    _type = type;

    auto blockBtn = Button::create(icon);
    blockBtn->setScale(defaultIconScale);
    blockBtn->setPosition(getContentSize() / 2);
    addChild(blockBtn);
    blockBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        auto blockBtn = dynamic_cast<Button*>(sender);
        auto blockWrapper = dynamic_cast<BlockWrapper*>(blockBtn->getParent());
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("block item clicked\n");
            _clickedCallback(this);
            if (PageViewItem::_clickedCallback) {
                PageViewItem::_clickedCallback(this);
            }
            break;
        default:
            break;
        }
    });

    return true;
}

void BlockWrapper::setClickedCallback(const blockClicked& cb) {
    _clickedCallback = cb;
}

bool BlockSelector::init() {
    if (!Layout::init()) {
        return false;
    }
    setName("block_selector");
    setContentSize(Size(defaultPanelWidth, defaultPanelHeight));

    setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    setBackGroundColor(defaultBackgroundColor);
    setBackGroundColorOpacity(defaultOpacity);

    auto lp = RelativeLayoutParameter::create();
    lp->setRelativeName(getName()); // 给组件布局属性设置一个名字，别人可以找到它
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL); // 置于屏幕正上方
    lp->setMargin(Margin(0, 0, gapFromWindowRight, 0));
    setLayoutParameter(lp);

    auto layout = Layout::create();
    //layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    //layout->setBackGroundColor(blueBackgroundColor);
    //layout->setBackGroundColorOpacity(defaultOpacity);

    layout->setLayoutType(Layout::Type::VERTICAL);
    layout->setContentSize(Size(gBlockButtonSize.width, defaultPanelHeight));
    addChild(layout);

    _blockViews = PageViewWindow::create();
    _blockViews->init(PageViewWindow::Direction::VERTICAL, gBlockButtonSize, blockIndicatorSize);
    layout->addChild(_blockViews);

    addMultiplesSelector(layout);

    addAllCreatableBlock();

    return true;
}

void BlockSelector::attachPlayer(Player* player) {
    _player = player;
}

void BlockSelector::addMultiplesSelector(Layout* layout) {
    // 按4,2,1的顺序
    std::vector<StackedButton::Item> items;
    auto item0 = StackedButton::Item {
        "images/block-icon-multiply-4.png",
        CC_CALLBACK_2(BlockSelector::onMultiplesChanged, this),
    };
    items.push_back(item0);
    auto item1 = StackedButton::Item {
        "images/block-icon-multiply-2.png",
        CC_CALLBACK_2(BlockSelector::onMultiplesChanged, this),
    };
    items.push_back(item1);
    auto item2 = StackedButton::Item {
        "images/block-icon-multiply-1.png",
        CC_CALLBACK_2(BlockSelector::onMultiplesChanged, this),
    };
    items.push_back(item2);
    _stackedButton = StackedButton::create(items);
    _stackedButton->setScale(defaultIconScale);
    layout->addChild(_stackedButton);
}

void BlockSelector::onMultiplesChanged(StackedButton* stackedButton, int index) {
    auto item = stackedButton->getItem(index);
    printf("[BlockSelector::onMultiplesChanged] index: %d, icon: %s\n", index, item->icon.c_str());
}

void BlockSelector::addAllCreatableBlock() {
    int beginBlock = (int)BlockType::WALL;
    int endBlock = (int)BlockType::IMMOVABLE_BLOCK; // not to include
    for (int blockType = beginBlock; blockType < endBlock; blockType++) {
        addBlock((BlockType)blockType);
    }
}

void BlockSelector::addBlock(BlockType type) {
    const char* icon = nullptr;
    switch (type) {
        case BlockType::WALL:
            icon = BlockWall::UIIcon;
            break;
        case BlockType::STONE:
            icon = BlockStone::UIIcon;
            break;
        case BlockType::FOREST:
            icon = BlockForest::UIIcon;
            break;
        case BlockType::BRIDGE:
            icon = BlockBridge::UIIcon;
            break;
        case BlockType::ICE:
            icon = BlockIce::UIIcon;
            break;
        case BlockType::RIVER:
            icon = BlockRiver::UIIcon;
            break;
        //case BlockType::BARRIER:
        //    icon = BlockBarrier::UIIcon;
        //    break;
        case BlockType::TRENCH:
            icon = BlockTrench::UIIcon;
            break;
        case BlockType::CLOUD:
            icon = BlockCloud::UIIcon;
            break;
        case BlockType::CAMPUS:
            icon = Campus::UIIcon;
            break;
        case BlockType::CAMP:
            icon = Camp::UIIcon;
            break;
        default:
            CCLOG("[BlockSelector::addBlock] ERROR: Unsupported block type: %d", type);
            break;
    }
    if (icon) {
        auto blockBtn = BlockWrapper::create(type, icon);
        blockBtn->setClickedCallback(CC_CALLBACK_1(BlockSelector::onBlockClicked, this));
        _blockViews->pushBackPage(blockBtn);
    }
}

void BlockSelector::onBlockClicked(BlockWrapper* blockBtn) {
    BlockType blockType = blockBtn->getBlockType();
    int blockMultiplesIndex = _stackedButton->getCurrentIndex();
    printf(">> BlockSelector::onBlockClicked, blockType: %d, multiples index: %d\n",
            blockType, blockMultiplesIndex);
    _player->choiceCreatingBlockType(blockType);
    switch (blockMultiplesIndex) {
        case BLOCK_4X:
            _player->createBlock4();
            break;
        case BLOCK_2X:
            _player->createBlock2();
            break;
        case BLOCK_1X:
            _player->createBlock1();
            break;
        default:
            break;
    }
}

void BlockSelector::scrollUp() {
    _blockViews->scrollForward();
}

void BlockSelector::scrollDown() {
    _blockViews->scrollBackward();
}

void BlockSelector::switchTo4X() {
    _stackedButton->select(BLOCK_4X);
}

void BlockSelector::switchTo2X() {
    _stackedButton->select(BLOCK_2X);
}

void BlockSelector::switchTo1X() {
    _stackedButton->select(BLOCK_1X);
}
