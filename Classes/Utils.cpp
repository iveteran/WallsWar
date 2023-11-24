#include "Utils.h"

static const double PI = 3.141592654;

bool isPointInCirle(const Vec2& center, float radius, const Vec2& point) {
	float x = point.x - center.x;
	float y = point.y - center.y;
	float distance = sqrt(pow(x, 2) + pow(y, 2));
	return distance <= radius;
}

// 获取以p1为圆心，p2p1与x轴正方向的弧度值
float calcRad(const Point& p1, const Point& p2) {
    float xx = p2.x - p1.x;
    float yy = p2.y - p1.y;

    // 斜边
    float xie = sqrt(pow(xx, 2) + pow(yy, 2));
    // yy >= 0 弧度在于 0 到 π 之间。(0~180°)
    // yy < 0 弧度在于 π 到 2π 之间。(180°~360°)
    float rad = yy >= 0 ? (acos(xx / xie)) : (PI * 2 - acos(xx / xie));
    return rad;
}

// 得到与角度对应的半径为R的圆上一坐标点, 相对值
Vec2 getAnglePosition(float R, float rad) {
    return Point(R * cos(rad), R * sin(rad));
}
