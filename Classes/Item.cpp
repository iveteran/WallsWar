#include "Item.h"

const char* getBagCategoryName(ItemCategory category) {
    const char* name = nullptr;
    switch (category) {
        case ItemCategory::PROP:
            name = "道具";
            break;
        case ItemCategory::AD:
            //name = "广告赞助";
            name = "赞助";
            break;
        default:
            break;
    }
    return name;
}
