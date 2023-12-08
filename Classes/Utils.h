#pragma once

#include "math/Vec2.h"

USING_NS_CC;

bool isPointInCircle(const Vec2& center, float radius, const Vec2& point);
float calcRad(const Point& p1, const Point& p2);
Vec2 getAnglePosition(float R, float rad);
