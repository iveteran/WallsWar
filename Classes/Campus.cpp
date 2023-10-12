#include "Campus.h"

USING_NS_CC;

void Campus::initSpriteFrameCache() {
    // TODO
}

bool Campus::init() {
    if (!ImmovableBlock::init()) {
        return false;
    }

    setFloor(Campus::DefaultFloor);
    initWithSpriteFrameName(getSpriteFrameName());

    return true;
}
