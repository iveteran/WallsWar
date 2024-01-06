#ifndef _ZOOM_OUT_MAP_H_
#define _ZOOM_OUT_MAP_H_

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/UIButton.h"

USING_NS_CC;

using namespace cocos2d::ui;

class Player;

enum class EntityType {
    NONE,
    MASTER,
    CAMPUS,
    GUEST,
    TEAMMATE,
    ENEMY,
};

class ZoomOutEntity : public Button {
public:
    static ZoomOutEntity* Master(const Vec2& pos);
    static ZoomOutEntity* Campus(const Vec2& pos);
    static ZoomOutEntity* CampusHollow(const Vec2& pos);
    static ZoomOutEntity* GuestCampus(const Vec2& pos);
    static ZoomOutEntity* CampusWithIcon(const Vec2& pos, const char* icon);
    static ZoomOutEntity* Guest(const Vec2& pos);

public:
    static ZoomOutEntity* create(const char* img, const Vec2& pos);
    bool init(const char* img, const Vec2& pos);

    void setType(EntityType type) { _type = type; }

private:
    EntityType _type;
};

class ZoomOutMap : public Widget {
public:
    CREATE_FUNC(ZoomOutMap);
    virtual bool init() override;

    void attachPlayer(Player* player) { _player = player; }
    void detachPlayer() { _player = nullptr; }
    void addEntity(ZoomOutEntity* entity);
    void addMaster();
    void addCampus();
    void addGuest(const Vec2& pos);
    void clear();

protected:
    void update(float dt);
    void scanPlayers();

private:
    LayerRadialGradient* _layer = nullptr;
    Vec2 _center;
    float _radius;
    float _scanningRadius;

    Player* _player = nullptr;
    float _accumDt = 0.0f;
};

#endif  // _ZOOM_OUT_MAP_H_
