#include "Utils.h"

static const double PI = 3.141592654;

bool isPointInCircle(const Vec2& center, float radius, const Vec2& point) {
	float x = point.x - center.x;
	float y = point.y - center.y;
	float distance = sqrt(pow(x, 2) + pow(y, 2));
	return distance <= radius;
}

bool isPointInRing(const Vec2& center, float outerRadius, float innerRadius, const Vec2& point) {
    return isPointInCircle(center, outerRadius, point) && !isPointInCircle(center, innerRadius, point);
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

float calculateDegreeOfTwoPoints(const Vec2& prevPoint, const Vec2& currPoint) {
    float x1 = currPoint.x;
    float y1 = currPoint.y;
    float x2 = prevPoint.x;
    float y2 = prevPoint.y;

    float mo = std::sqrt(x1 * x1 + y1 * y1) * std::sqrt(x2 * x2 + y2 * y2);
    if (mo == 0) {
        return 0.f;
    }
    float ji = x1 * x2 + y1 * y2;
    float degree = std::acos(ji / mo) / PI * 180.0;
    return degree;
}

// result is 0(equal), 1(large than) or -1(less than)
int compareTwoPointsDegree(const Vec2& p1, const Vec2& p2) {
    Vec2 xAxis(1, 0); // x轴，正向
    if (p2.y < 0) {
        xAxis.x *= -1; // 负向
    }
    float degree1 = calculateDegreeOfTwoPoints(p1, xAxis);
    float degree2 = calculateDegreeOfTwoPoints(p2, xAxis);
    return degree1 == degree2 ? 0 : (degree1 > degree2 ? 1 : -1);
}

float floatModInt(float v1, int v2) {
    int mod = (int)v1 % v2;
    float decimalPart = v1 - (int)v1;
    return mod + decimalPart;
}
