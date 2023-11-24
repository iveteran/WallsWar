#include "Campus.h"

USING_NS_CC;

bool isCampus(BlockType btype) {
    return btype == BlockType::CAMPUS ||
        btype == BlockType::CAMP;
}

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
