#ifndef _BLOCK_SELECTOR_H_
#define _BLOCK_SELECTOR_H_

#include "cocos2d.h"
#include "ui/UILayout.h"

#include "ImmovableBlock.h"
#include "PageViewWindow.h"

USING_NS_CC;

namespace cocos2d::ui {
    class Button;
    class ImageView;
}
using namespace cocos2d::ui;

class Player;
class PageViewWindow;
class StackedButton;
class BlockSelector;
class BlockWrapper;

using blockClicked = std::function<void(BlockWrapper*)>;

class BlockWrapper : public PageViewItem {
public:
    static BlockWrapper* create(BlockType type, const char* icon);
    bool init(BlockType type, const char* icon);

    void setClickedCallback(const blockClicked& cb);
    BlockType getBlockType() const { return _type; }

private:
    BlockType _type;
    Button* _blockIcon = nullptr;
    blockClicked _clickedCallback = nullptr;
};

class BlockSelector : public Layout {
public:
    enum Position {
        TOP,
        CENTER,
        BOTTOM,
    };

    CREATE_FUNC(BlockSelector);
    virtual bool init() override;

    void attachPlayer(Player* player);

    void scrollUp();
    void scrollDown();
    void switchTo4X();
    void switchTo2X();
    void switchTo1X();

protected:
    void addMultiplesSelector(Layout* layout);
    void onMultiplesChanged(StackedButton* stackedButton, int index);
    void onBlockClicked(BlockWrapper* blockBtn);

    void addAllCreatableBlock();
    void addBlock(BlockType type);

private:
    Player* _player = nullptr;
    PageViewWindow* _blockViews = nullptr;
    StackedButton* _stackedButton = nullptr;
};

#endif  // _BLOCK_SELECTOR_H_
