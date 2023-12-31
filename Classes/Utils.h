#pragma once

#include "math/Vec2.h"

USING_NS_CC;

float degreeToRadians(float degrees);
float radiansToDegree(float radians);
bool isPointInCircle(const Vec2& center, float radius, const Vec2& point);
bool isPointInRing(const Vec2& center, float outerRadius, float innerRadius, const Vec2& point);
float calcRad(const Point& p1, const Point& p2);
Vec2 getAnglePosition(float R, float rad);
float calculateDegreeOfTwoPoints(const Vec2& currPoint, const Vec2& prevPoint);
int compareTwoPointsDegree(const Vec2& p1, const Vec2& p2);
float floatModInt(float v1, int v2);
