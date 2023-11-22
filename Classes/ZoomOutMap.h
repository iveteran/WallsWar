#ifndef _ZOOM_OUT_MAP_H_
#define _ZOOM_OUT_MAP_H_

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/UIButton.h"

USING_NS_CC;

using namespace cocos2d::ui;

class ZoomOutEntity : public Button {
public:
    static ZoomOutEntity* Master();
    static ZoomOutEntity* Guest();

public:
    static ZoomOutEntity* create(const char* img);
    bool init(const char* img);

    void setMaster();
    void setGuest();
};

class ZoomOutMap : public Widget {
public:
    CREATE_FUNC(ZoomOutMap);
    virtual bool init() override;

    void addEntity(ZoomOutEntity* entity);

private:
    LayerRadialGradient* _layer = nullptr;
};

#endif  // _ZOOM_OUT_MAP_H_
