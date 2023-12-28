#ifndef _WEAPON_WHEEL_H_
#define _WEAPON_WHEEL_H_

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/UIButton.h"

#include "Block.h"

USING_NS_CC;

namespace cocos2d::ui {
    class Text;
    class ImageView;
}
using namespace cocos2d::ui;

class Player;

static const int MAX_SLOTS_NUM = 8;

class WeaponEmitter : public Widget {
public:
    static WeaponEmitter* Empty(int slot);
    static WeaponEmitter* Bomb(int slot);
    static WeaponEmitter* SmokeBomb(int slot);
    static WeaponEmitter* FlashBomb(int slot);
    static WeaponEmitter* FireBomb(int slot);
    static WeaponEmitter* Landmine(int slot);
    static WeaponEmitter* Axe(int slot);
    static WeaponEmitter* Spade(int slot);

public:
    static WeaponEmitter* create(BlockType type, const char* icon, int index);
    bool init(BlockType type, const char* icon, int index);

    void attachCountLabel(Text* countLabel);
    void rotateIconBy(float degree);

    bool isEmpty() const { return _type == BlockType::UNDEFINED; }
    int increase(int num = 1);
    int decrease(int num = 1);
    int emit();

protected:
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void updateCountLabel();

private:
    Button* _btn = nullptr;
    Text* _countLabel = nullptr;
    int _slot;
    BlockType _type;
    int _count = 0;
};

class WeaponWheel : public Widget {
    friend class WeaponEmitter;
public:
    CREATE_FUNC(WeaponWheel);
    virtual bool init() override;

    void attachPlayer(Player* player) { _player = player; }

    bool add(BlockType type, int slot);
    bool add(BlockType type);
    void remove(int slot);

    void clear();

protected:
    Vec2 convertToNodeLocation(const Vec2& point);
    void rotateAllWeaponsBy(float degree);
    Vec2 calculatePositionForWeapon(int slot);
    Vec2 calculatePositionForWeaponCount(int slot);
    Vec2 calculatePositionForWheelSlot(int slot, float radiusOffset, float degreeOffset);
    int getNextEmptySlot(int start = 0);

    void rotateByGrid(int unit=1);
    void rotateByDegree(float degree, float duration=0.f);
    void update(float dt);
    float _accumDt = 0;

    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);

private:
    int _capacity = MAX_SLOTS_NUM;
    Vec2 _center;
    float _radius;
    float _innerRadius;
    //Sprite* _weaponWheel = nullptr;
    ImageView* _weaponWheel = nullptr;
    Vector<WeaponEmitter*> _slots;
    Vec2 _prevMovingPoint;
    float _accumDegree = 0.f;
    float _offsetDegree = 0.f;

    Player* _player = nullptr;
};

#endif  // _WEAPON_WHEEL_H_
