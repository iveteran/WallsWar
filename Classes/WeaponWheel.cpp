#include "WeaponWheel.h"
#include "Player.h"
#include "Utils.h"

#include "ui/UIText.h"
#include "ui/UIImageView.h"

static const char* defaultFont = "fonts/simhei.ttf";
static int defaultFontSize = 14;
static const Color4B defaultFontColor = Color4B(200, 200, 200, 128);  // near gray
static const float defaultRadius = 30.0f;
static const float defaultIconScale = 0.3f;
static const int AGREE_OF_ONE_UNIT = 45;
static const float AGREE_OF_HALF_UNIT = 22.5;

float getAgreeByWheelSlot(int slot);

WeaponEmitter* WeaponEmitter::Empty(int slot) {
    return WeaponEmitter::create(BlockType::UNDEFINED, "images/icon-weapon-empty.png", slot);
}

WeaponEmitter* WeaponEmitter::Bomb(int slot) {
    return WeaponEmitter::create(BlockType::BOMB, "images/icon-weapon-bomb.png", slot);
}

WeaponEmitter* WeaponEmitter::SmokeBomb(int slot) {
    return WeaponEmitter::create(BlockType::SMOKE_BOMB, "images/icon-weapon-smoke-bomb.png", slot);
}

WeaponEmitter* WeaponEmitter::FlashBomb(int slot) {
    return WeaponEmitter::create(BlockType::FLASH_BOMB, "images/icon-weapon-flash-bomb.png", slot);
}

WeaponEmitter* WeaponEmitter::FireBomb(int slot) {
    return WeaponEmitter::create(BlockType::FIRE_BOMB, "images/icon-weapon-fire-bomb.png", slot);
}

WeaponEmitter* WeaponEmitter::Landmine(int slot) {
    return WeaponEmitter::create(BlockType::LANDMINE, "images/icon-weapon-landmine.png", slot);
}

WeaponEmitter* WeaponEmitter::Axe(int slot) {
    return WeaponEmitter::create(BlockType::AXE, "images/icon-weapon-axe.png", slot);
}

WeaponEmitter* WeaponEmitter::Spade(int slot) {
    return WeaponEmitter::create(BlockType::SPADE, "images/icon-weapon-spade.png", slot);
}

WeaponEmitter* WeaponEmitter::create(BlockType type, const char* icon, int slot) {
    auto *pRet = new(std::nothrow) WeaponEmitter();
    if (pRet && pRet->init(type, icon, slot)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

bool WeaponEmitter::init(BlockType type, const char* icon, int slot) {
    if (!Widget::init()) {
        return false;
    }
    setContentSize(Size(50, 50));

    _type = type;
    _slot = slot;
    _btn = Button::create(icon);
    _btn->setScale(0.7);
    _btn->setPosition(getContentSize() / 2);
    addChild(_btn);
    _btn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        //auto self = dynamic_cast<WeaponEmitter*>(sender);
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            printf("Weapon emitter clicked, type: %d, slot: %d\n", _type, _slot);
            break;
        default:
            break;
        }
    });

    // 触摸监听,单点触控
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(WeaponEmitter::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(WeaponEmitter::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(WeaponEmitter::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

bool WeaponEmitter::onTouchBegan(Touch* touch, Event* event) {
    //printf("WeaponEmitter::onTouchBegan\n");
    auto weaponWheel = dynamic_cast<WeaponWheel*>(getParent()->getParent());
    weaponWheel->onTouchBegan(touch, event);
    return true;
}

void WeaponEmitter::onTouchMoved(Touch* touch, Event* event) {
    //printf("WeaponEmitter::onTouchMoved\n");
    auto weaponWheel = dynamic_cast<WeaponWheel*>(getParent()->getParent());
    weaponWheel->onTouchMoved(touch, event);
}

void WeaponEmitter::onTouchEnded(Touch* touch, Event* event) {
    //printf("WeaponEmitter::onTouchEnded\n");
    auto weaponWheel = dynamic_cast<WeaponWheel*>(getParent()->getParent());
    weaponWheel->onTouchEnded(touch, event);
}

void WeaponEmitter::attachCountLabel(Text* countLabel) {
    _countLabel = countLabel;
    updateCountLabel();
}

void WeaponEmitter::updateCountLabel() {
    if (_countLabel) {
        _countLabel->setString(StringUtils::format("%d", _count));
    }
}

void WeaponEmitter::rotateIconBy(float agree) {
    float duration = 0.f;
    auto rotateBy = RotateBy::create(duration, agree);
    _btn->runAction(rotateBy);
    if (_countLabel) {
        auto rotateBy = RotateBy::create(duration, agree);
        _countLabel->runAction(rotateBy);
    }
}

int WeaponEmitter::increase(int num) {
    _count += num;
    updateCountLabel();
    return _count;
}

int WeaponEmitter::decrease(int num) {
    _count = num > _count ? 0 : _count - num;
    updateCountLabel();
    return _count;
}

int WeaponEmitter::emit() {
    return decrease();
}

bool WeaponWheel::init() {
    if (!Widget::init()) {
        return false;
    }
    setName("weapon_wheel");
    setContentSize(Size(defaultRadius, defaultRadius) * 2);

    auto visibleSize = Director::getInstance()->getVisibleSize();

    _center = getContentSize() / 2;
    _radius = defaultRadius;
    _innerRadius = _radius / 2;

    for (int slot=0; slot<_capacity; slot++) {
        _slots.pushBack(WeaponEmitter::Empty(slot));
    }

    _weaponWheel = ImageView::create("images/joypad/weapon_wheel.png"); // alpha 50%
    _weaponWheel->setScale(defaultIconScale);
    _weaponWheel->setPosition(getContentSize() / 2);
    addChild(_weaponWheel);

    // 开火键
    auto attack = Button::create("images/joypad/shoot.png");
    attack->setScale(0.3);
    attack->setPosition(getContentSize() / 2);
    addChild(attack);
    attack->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::ENDED:
            if (_player != nullptr) {
                _player->shoot();
            } else {
                CCLOG("[WARNING] the joypad does not attach player");
            }
            break;
        default:
            break;
        }
    });

    add(BlockType::BOMB);
    add(BlockType::SMOKE_BOMB);
    add(BlockType::FLASH_BOMB);
    add(BlockType::FIRE_BOMB);
    add(BlockType::LANDMINE);
    add(BlockType::AXE);
    add(BlockType::SPADE);

    //scheduleUpdate();

    // 触摸监听，用于将在WeaponEmitter图标上的拖动事件传递给WeaponWheel
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(WeaponWheel::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(WeaponWheel::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(WeaponWheel::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

Vec2 WeaponWheel::convertToNodeLocation(const Vec2& point) {
    return Vec2(point.x - getPosition().x, point.y - getPosition().y);
}

void WeaponWheel::rotateAllWeaponsBy(float agree) {
    for (auto item : _slots) {
        item->rotateIconBy(agree);
    }
}

bool WeaponWheel::onTouchBegan(Touch* touch, Event* event) {
    auto point = convertToNodeLocation(touch->getLocation());
    Vec2 center(0,0);
    if (!isPointInRing(center, _radius, _innerRadius, point)) {
        return true;
    }
    _prevMovingPoint = point;
    _accumAgree = 0;

    return true;
}

void WeaponWheel::onTouchMoved(Touch* touch, Event* event) {
    auto point = convertToNodeLocation(touch->getLocation());
    Vec2 center(0,0);
    if (!isPointInRing(center, _radius, _innerRadius, point)) {
        return;
    }
    if (point == _prevMovingPoint) {
        return;
    }

    //printf(">> touch prev point: (%f, %f)\n", _prevMovingPoint.x, _prevMovingPoint.y);
    //printf(">> touch moving point: (%f, %f)\n", point.x, point.y);
    float agree = calculateAgreeOfTwoPoints(_prevMovingPoint, point);
    //printf(">> touch moving agree: %f\n", agree);
    if (agree == 0) {
        return;
    }
    // result is 0(equal), 1(large than) or -1(less than)
    int direction = compareTwoPointsAgree(_prevMovingPoint, point);
    //printf(">> touch moving agree direction: %d\n", direction);
    float agreeDelta = agree * direction;
    //printf("?? rotate agree: %f\n", agreeDelta);
    rotateByAgree(agreeDelta);
    _accumAgree += agreeDelta;
    _prevMovingPoint = point;
}

void WeaponWheel::onTouchEnded(Touch* touch, Event* event) {
    auto point = convertToNodeLocation(touch->getLocation());
    Vec2 center(0,0);
    if (!isPointInRing(center, _radius, _innerRadius, point)) {
        return;
    }
    //printf(">> touch end point: (%f, %f)\n", point.x, point.y);

    // 确保以一个或多个单元格进行滚动
    int direction = 1;
    if (_accumAgree < 0) {
        direction = -1;
    }
    float accumAgreeAbs = abs(_accumAgree);
    float mod = floatModInt(accumAgreeAbs, AGREE_OF_ONE_UNIT);
    if (mod == 0) {
        return;
    }
    float agree = 0;
    if (mod < AGREE_OF_HALF_UNIT) {
        agree = mod * -1 * direction; // 小于半个单元格，回滚
    } else if (mod >= AGREE_OF_HALF_UNIT) {
        agree = (AGREE_OF_ONE_UNIT - mod) * direction;  // 大于半个半元格，补齐一个单元格
    }
    //printf("?? _accumAgree: %f, mod: %f, rotate agree: %f\n", _accumAgree, mod, agree);
    rotateByAgree(agree);
    _accumAgree = 0;
}

bool WeaponWheel::add(BlockType type, int slot) {
    if (slot < 0 || slot >= _slots.size()) {
        CCLOG("ERROR [WheaponWheel]: the slot is out of range");
        return false;
    }
    if (!_slots.at(slot)->isEmpty()) {
        // the slot has taken
        CCLOG("ERROR [WheaponWheel]: the slots of weapon wheel is full");
        return false;
    }

    WeaponEmitter* weaponEmitter = nullptr;
    switch (type) {
        case BlockType::BOMB:
            weaponEmitter = WeaponEmitter::Bomb(slot);
            weaponEmitter->increase(5); // test code
            break;
        case BlockType::SMOKE_BOMB:
            weaponEmitter = WeaponEmitter::SmokeBomb(slot);
            weaponEmitter->increase(10); // test code
            break;
        case BlockType::FLASH_BOMB:
            weaponEmitter = WeaponEmitter::FlashBomb(slot);
            break;
        case BlockType::FIRE_BOMB:
            weaponEmitter = WeaponEmitter::FireBomb(slot);
            break;
        case BlockType::LANDMINE:
            weaponEmitter = WeaponEmitter::Landmine(slot);
            break;
        case BlockType::AXE:
            weaponEmitter = WeaponEmitter::Axe(slot);
            weaponEmitter->increase(1); // test code
            break;
        case BlockType::SPADE:
            weaponEmitter = WeaponEmitter::Spade(slot);
            weaponEmitter->increase(1); // test code
            break;
        default:
            CCLOG("ERROR [WheaponWheel]: unsupported weapon type: %d", type);
            break;
    }
    if (!weaponEmitter) {
        return false;
    }
    auto pos = calculatePositionForWeapon(slot);
    //printf(">> WeaponWheel center at pos(%f, %f)\n", _center.x, _center.y);
    //printf(">> WeaponWheel adds item at pos(%f, %f)\n", pos.x, pos.y);
    weaponEmitter->setPosition(pos / defaultIconScale);
    _weaponWheel->addChild(weaponEmitter);

    auto weaponCountPos = calculatePositionForWeaponCount(slot);
    auto countLabel = Text::create("", defaultFont, defaultFontSize);
    //countLabel->setTextColor(defaultFontColor);
    countLabel->setPosition(weaponCountPos / defaultIconScale);
    weaponEmitter->attachCountLabel(countLabel);
    _weaponWheel->addChild(countLabel);

    _slots.insert(slot, weaponEmitter);

    return true;
}

bool WeaponWheel::add(BlockType type) {
    int slot = getNextEmptySlot();
    if (slot >= 0) {
        add(type, slot);
        return true;
    }
    return false;
}

void WeaponWheel::remove(int slot) {
    if (_slots.at(slot)->isEmpty()) {
        return;
    }
    _slots.insert(slot, WeaponEmitter::Empty(slot));
}

void WeaponWheel::rotateByGrid(int unit) {
    float duration = 0.5f;
    float agree = AGREE_OF_ONE_UNIT * unit;
    rotateByAgree(agree, duration);
}

void WeaponWheel::rotateByAgree(float agree, float duration) {
    if (agree == 0) {
        return;
    }
    // duration is zero means to do immediately
    auto rotateBy = RotateBy::create(duration, agree);
    _weaponWheel->runAction(rotateBy);
    rotateAllWeaponsBy(agree * -1); // 和滚轮滚动方向相反
    _offsetAgree = floatModInt(_offsetAgree + agree, 360);
    //printf("?? agree: %f, _offsetAgree: %f, offset slots: %f\n",
    //        agree, _offsetAgree, _offsetAgree / AGREE_OF_ONE_UNIT);
}

int WeaponWheel::getNextEmptySlot(int start) {
    for (int slot = start; slot < _slots.size(); slot++) {
        if (_slots.at(slot)->isEmpty()) {
            return slot;
        }
    }
    return -1;
}

void WeaponWheel::clear() {
    _weaponWheel->removeAllChildren();
    _slots.clear();
}

// Just for test rotating weapon wheel
void WeaponWheel::update(float dt) {
    _accumDt += dt;

    if (_accumDt > 1) {
        rotateByGrid();
        _accumDt = 0;
    }
}

Vec2 WeaponWheel::calculatePositionForWeapon(int slot) {
    float radiusOffset = _radius / 4 * -1;
    return calculatePositionForWheelSlot(slot, radiusOffset, 0);
}

Vec2 WeaponWheel::calculatePositionForWeaponCount(int slot) {
    float radiusOffset = -2;
    float agreeOffset = 18;
    return calculatePositionForWheelSlot(slot, radiusOffset, agreeOffset);
}

Vec2 WeaponWheel::calculatePositionForWheelSlot(int slot, float radiusOffset, float agreeOffset) {
    Vec2 pos;
    float agree = getAgreeByWheelSlot(slot);
    if (agree < 0) {
        return pos;
    }
    agree += agreeOffset;
    float r = _radius + radiusOffset;
    pos.x = _center.x + r * std::cos(CC_DEGREES_TO_RADIANS(agree));
    pos.y = _center.y + r * std::sin(CC_DEGREES_TO_RADIANS(agree));
    return pos;
}

// 将圆中8等份的slot转为角度，从45分钟处开始间隔90分钟
float getAgreeByWheelSlot(int slot) {
    float agree = -1.0f;
    switch (slot) {
        case 0:
            agree = 90 - AGREE_OF_HALF_UNIT; // 时钟45分钟处
            break;
        case 1:
            agree = 0 + AGREE_OF_HALF_UNIT;  // 时钟2:15处
            break;
        case 2:
            agree = 360 - AGREE_OF_HALF_UNIT; // 时钟3:45处
            break;
        case 3:
            agree = 270 + AGREE_OF_HALF_UNIT;
            break;
        case 4:
            agree = 270 - AGREE_OF_HALF_UNIT;
            break;
        case 5:
            agree = 180 + AGREE_OF_HALF_UNIT;
            break;
        case 6:
            agree = 180 - AGREE_OF_HALF_UNIT;
            break;
        case 7:
            agree = 90 + AGREE_OF_HALF_UNIT;
            break;
        default:
            break;
    }
    return agree;
}
