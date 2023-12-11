#ifndef _BAGS_PANEL_H_
#define _BAGS_PANEL_H_

#include "cocos2d.h"
#include "ui/UILayout.h"

#include "Item.h"

USING_NS_CC;

namespace cocos2d::ui {
    class Text;
    class ListView;
}
using namespace cocos2d::ui;

enum class SizeChange {
    DECREASE = -1,
    INCREASE = 1,
};

class Bag : public Layout {
public:
    static Bag* create(ItemType type, const char* name, const char* icon);
    bool init(ItemType type, const char* name, const char* icon);

    int addItem(Item* item);
    int dropItem(int itemId);
    bool isEmpty() const { return _items.empty(); }
    int count() const { return _items.size(); }

private:
    ItemCategory _category;
    ItemType _type;
    Map<int, Item*> _items;

    Text* _countLabel = nullptr;
};

class BagsPanel : public Layout {
friend class Bag;
public:
    static BagsPanel* create(ItemCategory category=ItemCategory::PROP);
    bool init(ItemCategory category);

    int addItem(Item* item);
    int dropItem(int itemId);

    bool isEmpty() const { return _bags.empty(); }
    int count() const { return _bags.size(); }

protected:
    void addBag(ItemType type, const char* name, const char* icon, int index=-1);
    void removeBag(ItemType type);

    void updateWidth(SizeChange direction);

private:
    ItemCategory _category;
    Map<ItemType, Bag*> _bags;
    int _maxShowCount;

    Map<int, Item*> _items;
    ListView* _bagsView = nullptr;
};

class MultiBagsPanel : public Layout {
friend class BagsPanel;
public:
    CREATE_FUNC(MultiBagsPanel);
    bool init() override;

    int addItem(Item* item);
    int dropItem(int itemId);

    int count() const { return _bagsPanels.size(); }

protected:
    void addBagsPanel(ItemCategory, int index=-1);
    void removeBagsPanel(ItemCategory type);

    void updateWidth();

private:
    Map<int, Item*> _items;
    Map<ItemCategory, BagsPanel*> _bagsPanels;

    ListView* _bagsPanelsView = nullptr;
};

#endif  // _BAGS_PANEL_H_
