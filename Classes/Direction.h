#pragma once

// 移动方向，采用顺时针排列，NOTE: 不要改变它们的顺序
enum class Direction {
    NONE = -1, LEFT, UP, RIGHT, DOWN, COUNT
};

enum class RRDirection {
    FORWARD, BACKWARD
};

Direction reverseDirection(Direction dir);
