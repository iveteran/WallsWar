#pragma once

#include "ImmovableBlock.h"

enum class ItemType {
    NONE,
    WOOD,
    SOIL,
    MINERAL,
    IRON,
    GUNPOWDER,
    ADCARD,
};

enum class ItemCategory {
    NONE,
    PROP,
    AD,
};

const char* getBagCategoryName(ItemCategory category);

class Item : public ImmovableBlock {
public:
    bool init() override { return ImmovableBlock::init(); }
    virtual BlockType getType() const override { return BlockType::ITEM; }

    virtual ItemCategory getItemCategory() const { return ItemCategory::PROP; }
    virtual ItemType getItemType() const { return ItemType::NONE; }
    virtual const char* getItemName() const = 0;
    virtual const char* getIcon() const = 0;
    //virtual ImmovableBlock* makeFrom() const = 0;
};

class Wood : public Item {
public:
    static const constexpr char* Name = "Wood";
    static const constexpr char* Icon = "images/icon-item-wood.png";

public:
    CREATE_FUNC(Wood);

    bool init() override { return Item::init(); }

    ItemType getItemType() const { return ItemType::WOOD; }
    const char* getItemName() const { return Name; }
    const char* getIcon() const { return Icon; }
};

class Soil : public Item {
public:
    static const constexpr char* Name = "Soil";
    static const constexpr char* Icon = "images/icon-item-soil.png";

public:
    CREATE_FUNC(Soil);

    bool init() override { return Item::init(); }

    ItemType getItemType() const { return ItemType::SOIL; }
    const char* getItemName() const { return Name; }
    const char* getIcon() const { return Icon; }
};

class Mineral : public Item {
public:
    static const constexpr char* Name = "Mineral";
    static const constexpr char* Icon = "images/icon-item-mineral.png";

public:
    CREATE_FUNC(Mineral);

    bool init() override { return Item::init(); }

    ItemType getItemType() const { return ItemType::MINERAL; }
    const char* getItemName() const { return Name; }
    const char* getIcon() const { return Icon; }
};

class Iron : public Item {
public:
    static const constexpr char* Name = "Iron";
    static const constexpr char* Icon = "images/icon-item-iron.png";

public:
    CREATE_FUNC(Iron);

    bool init() override { return Item::init(); }

    ItemType getItemType() const { return ItemType::IRON; }
    const char* getItemName() const { return Name; }
    const char* getIcon() const { return Icon; }
};

class Gunpowder : public Item {
public:
    static const constexpr char* Name = "Gunpowder";
    static const constexpr char* Icon = "images/icon-item-gunpowder.png";

public:
    CREATE_FUNC(Gunpowder);

    bool init() override { return Item::init(); }

    ItemType getItemType() const { return ItemType::GUNPOWDER; }
    const char* getItemName() const { return Name; }
    const char* getIcon() const { return Icon; }
};

class AdCard : public Item {
public:
    static const constexpr char* Name = "AdCard";
    static const constexpr char* Icon = "images/icon-item-adcard.png";

public:
    CREATE_FUNC(AdCard);

    bool init() override { return Item::init(); }

    ItemType getItemType() const { return ItemType::ADCARD; }
    virtual ItemCategory getItemCategory() const { return ItemCategory::AD; }
    const char* getItemName() const { return Name; }
    const char* getIcon() const { return Icon; }
};
