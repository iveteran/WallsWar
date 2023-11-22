#include "ZoomOutMap.h"

#include "2d/CCLayer.h"

static const float defaultRadius = 30.0f;
static const Color4B defaultStartColor = Color4B(51, 51, 51, 128);  // near gray
static const Color4B defaultEndColor = Color4B(0, 0, 0, 0);
static const float defaultExpand = 1.0;

void addDemoEntities(bool isMaster, const Vec2& pos, ZoomOutMap* zoMap) {
    auto entity = isMaster ? ZoomOutEntity::Master() : ZoomOutEntity::Guest();
    entity->setPosition(pos);
    zoMap->addEntity(entity);
}

ZoomOutEntity* ZoomOutEntity::Master() {
    return ZoomOutEntity::create("images/zoomout-entity-master.png");
}

ZoomOutEntity* ZoomOutEntity::Guest() {
    return ZoomOutEntity::create("images/zoomout-entity-guest.png");
}

ZoomOutEntity* ZoomOutEntity::create(const char* img) {
    auto *pRet = new(std::nothrow) ZoomOutEntity();
    if (pRet && pRet->init(img)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool ZoomOutEntity::init(const char* img) {
    if (!Button::init(img)) {
        return false;
    }
    setScale(0.5);

    addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("Zoom out entity clicked\n");
            break;
        default:
            break;
        }
    });

    return true;
}

bool ZoomOutMap::init() {
    if (!Widget::init()) {
        return false;
    }
    setName("zoom_out_map");
    setContentSize(Size(defaultRadius, defaultRadius) * 2);

    auto lp = RelativeLayoutParameter::create();
    lp->setRelativeName(getName()); //给组件布局属性设置一个名字，别人可以找到它
    // 对齐status bar左下角
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::LOCATION_BELOW_LEFTALIGN);
    lp->setRelativeToWidgetName("status_bar"); //设定当前组件要与哪个组件对齐
    setLayoutParameter(lp);

    Vec2 center(getContentSize() / 2);
    _layer = LayerRadialGradient::create(defaultStartColor, defaultEndColor,
            defaultRadius, center, defaultExpand);
    addChild(_layer);

    addDemoEntities(true, center, this);
    addDemoEntities(false, Vec2(center.x + 8, center.y + 8), this);

    return true;
}

void ZoomOutMap::addEntity(ZoomOutEntity* entity) {
    _layer->addChild(entity);
}
