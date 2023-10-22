#pragma once

// 以下为设计尺寸

// 窗口大小
constexpr float WINDOW_WIDTH = 256;
constexpr float WINDOW_HEIGHT = 224;

// 箭头位置
constexpr float ARROWS_X = 75;
constexpr float ARROWS_Y = 132;
constexpr float ARROWS_DIS = 15;

// 中间游戏区域大小，注：BLOCK_SIZE的倍数
//constexpr float CENTER_WIDTH = 512;
//constexpr float CENTER_HEIGHT = 512;
constexpr float CENTER_WIDTH = 256;
constexpr float CENTER_HEIGHT = 224;

// 方块大小
constexpr float BLOCK_SIZE = 8;
constexpr float MAX_BLOCK_SIZE = 16;

// 砖块大小，墙(Wall)由4个砖块构成
constexpr float BRICK_SIZE = BLOCK_SIZE / 2;

// 坦克大小
constexpr float TANK_SIZE = 2 * BLOCK_SIZE;

/*
// 玩家坦克初始位置
constexpr float PLAYER1_START_X = 4 * BLOCK_SIZE;
constexpr float PLAYER1_START_Y = 1 * BLOCK_SIZE;

constexpr float PLAYER2_START_X = 1 * BLOCK_SIZE;
constexpr float PLAYER2_START_Y = 4 * BLOCK_SIZE;

// 敌方坦克初始位置
constexpr float ENEMY1_STAR_X = CENTER_WIDTH - 8;
constexpr float ENEMY1_STAR_Y = 8;

constexpr float ENEMY2_STAR_X = CENTER_WIDTH - 16;
constexpr float ENEMY2_STAR_Y = CENTER_HEIGHT - 16;

constexpr float ENEMY3_STAR_X = 8;
constexpr float ENEMY3_STAR_Y = CENTER_HEIGHT - 8;
*/

// 大本营位置
constexpr float CAMP_SIZE = BLOCK_SIZE * 2;
// 左下角
constexpr float CAMP_X = CAMP_SIZE / 2;
constexpr float CAMP_Y = CAMP_SIZE / 2;
// 右上角
constexpr float ENEMY_CAMP_X = CENTER_WIDTH - CAMP_SIZE / 2;
constexpr float ENEMY_CAMP_Y = CENTER_HEIGHT - CAMP_SIZE / 2;

// 子弹大小
constexpr float BULLET_SIZE = 4;

// 敌方坦克总数
constexpr unsigned char ENEMIES_COUNT = 20;

// 敌方坦克移动100距离后可更改方向
constexpr int MAX_MOVE_DISTANCE = 100;

// 关卡总数
constexpr short STAGE_COUNT = 35;

// 移动方向，采用顺时针排列，NOTE: 不要改变它们的顺序
enum class Direction {
    NONE = -1, LEFT, UP, RIGHT, DOWN, COUNT
};

enum class RRDirection {
    FORWARD, BACKWARD
};

// functions
Direction reverseDirection(Direction dir);
