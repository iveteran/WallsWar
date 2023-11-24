#include "ZoomOutMap.h"
#include "Player.h"
#include "Camp.h"
#include "GameScene.h"
#include "MapLayer.h"
#include "Utils.h"

#include "2d/CCLayer.h"

static const int ZOOM_OUT_SCANNING_MULTIPLE = 2;
static const float STATS_INTERVAL_FEW_SECONDS = 3;  // seconds
static const float defaultRadius = 30.0f;
static const Color4B defaultStartColor = Color4B(51, 51, 51, 128);  // near gray
static const Color4B defaultEndColor = Color4B(0, 0, 0, 0);
static const float defaultExpand = 1.0;

ZoomOutEntity* ZoomOutEntity::Master(const Vec2& pos) {
    auto entity = ZoomOutEntity::create("images/zoomout-entity-master.png", pos);
    entity->setType(EntityType::MASTER);
    entity->setScale(0.5); // master scale is 0.5
    return entity;
}

ZoomOutEntity* ZoomOutEntity::Campus(const Vec2& pos) {
    return CampusWithIcon(pos, "images/zoomout-entity-campus.png");
}

ZoomOutEntity* ZoomOutEntity::CampusHollow(const Vec2& pos) {
    return CampusWithIcon(pos, "images/zoomout-entity-campus-hollow.png");
}

ZoomOutEntity* ZoomOutEntity::GuestCampus(const Vec2& pos) {
    return CampusWithIcon(pos, "images/zoomout-entity-guest-campus.png");
}

ZoomOutEntity* ZoomOutEntity::CampusWithIcon(const Vec2& pos, const char* icon) {
    auto entity = ZoomOutEntity::create(icon, pos);
    entity->setType(EntityType::CAMPUS);
    entity->setScale(0.5); // campus scale is 0.5
    return entity;
}

ZoomOutEntity* ZoomOutEntity::Guest(const Vec2& pos) {
    auto entity = ZoomOutEntity::create("images/zoomout-entity-guest.png", pos);
    entity->setType(EntityType::GUEST);
    return entity;
}

ZoomOutEntity* ZoomOutEntity::create(const char* img, const Vec2& pos) {
    auto *pRet = new(std::nothrow) ZoomOutEntity();
    if (pRet && pRet->init(img, pos)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool ZoomOutEntity::init(const char* img, const Vec2& pos) {
    if (!Button::init(img)) {
        return false;
    }
    setScale(0.3); // set default scale
    setPosition(pos);

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

    auto visibleSize = Director::getInstance()->getVisibleSize();

    _center = getContentSize() / 2;
    _radius = defaultRadius;
    _scanningRadius = std::min(visibleSize.width, visibleSize.height) * ZOOM_OUT_SCANNING_MULTIPLE;

    auto lp = RelativeLayoutParameter::create();
    lp->setRelativeName(getName()); //给组件布局属性设置一个名字，别人可以找到它
    // 对齐status bar左下角
    lp->setAlign(RelativeLayoutParameter::RelativeAlign::LOCATION_BELOW_LEFTALIGN);
    lp->setRelativeToWidgetName("status_bar"); //设定当前组件要与哪个组件对齐
    setLayoutParameter(lp);

    _layer = LayerRadialGradient::create(defaultStartColor, defaultEndColor,
            _radius, _center, defaultExpand);
    addChild(_layer);

    addMaster();
    addCampus();

    // 默认渲染更新
    scheduleUpdate();

    return true;
}

void ZoomOutMap::addEntity(ZoomOutEntity* entity) {
    _layer->addChild(entity);
}

void ZoomOutMap::addMaster() {
    auto entity = ZoomOutEntity::Master(_center);
    addEntity(entity);
}

void ZoomOutMap::addCampus() {
    if (!_player) {
        return;
    }
    ZoomOutEntity* zoEntity = nullptr;
    auto playerPos = _player->getPosition();
    auto campusPos = _player->getCamp()->getPosition();
    auto zoomOutPos = (campusPos - playerPos) * (_radius / _scanningRadius) + Vec2(_radius, _radius);
    if (isPointInCirle(_center, _radius, zoomOutPos)) {
        zoEntity = ZoomOutEntity::Campus(zoomOutPos);
    } else {
        // calculate the point at cicle edge nearest from campus position
        float rad = calcRad(_center, zoomOutPos);
        zoomOutPos = _center + getAnglePosition(_radius, rad);
        zoEntity = ZoomOutEntity::CampusHollow(zoomOutPos);
    }
    addEntity(zoEntity);
}

void ZoomOutMap::addGuest(const Vec2& pos) {
    auto entity = ZoomOutEntity::Guest(pos);
    addEntity(entity);
}

void ZoomOutMap::clear() {
    _layer->removeAllChildren(); 
    addMaster();
    addCampus();
}

void ZoomOutMap::update(float dt) {
    _accumDt += dt;

    if (_accumDt > STATS_INTERVAL_FEW_SECONDS) {
        clear();
        scanPlayers();
        _accumDt = 0;
    }
}

void ZoomOutMap::scanPlayers() {
    if (!_player) {
        return;
    }
    auto mapLayer = GET_MAP_LAYER();
    if (!mapLayer) {
        return;
    }

    auto playerPos = _player->getPosition();
    BlockTypeSet btSet;
    btSet.insert(BlockType::PLAYER);
    btSet.insert(BlockType::CAMPUS);
    btSet.insert(BlockType::CAMP);
    CoordBlockMap blocks = mapLayer->getInsideCircleBlocks(playerPos, _scanningRadius, btSet);
    //printf(">> got around (radius: %f) players: %d\n", _scanningRadius, blocks.size());
    for (auto iter : blocks) {
        auto pos = iter.first;
        auto block = iter.second;
        if (block->id() == _player->id() || block->id() == _player->getCamp()->id()) {
            // 如果是玩家自己或自己的Campus/Camp
            continue;
        }
        //printf(">> scanned player/campus: %d, type: %d, position: (%f, %f)\n",
        //        block->id(), block->getType(), pos.x, pos.y);

        auto zoomOutPos = (pos - playerPos) * (_radius / _scanningRadius) + Vec2(_radius, _radius);
        if (block->getType() == BlockType::PLAYER) {
            auto zoEntity = ZoomOutEntity::Guest(zoomOutPos);
            addEntity(zoEntity);
        } else if (isCampus(block->getType())) {
            auto zoEntity = ZoomOutEntity::GuestCampus(zoomOutPos);
            addEntity(zoEntity);
        }
    }
}
