#pragma once

#include "BlockConstants.h"
#include "MapConstants.h"

constexpr float CAMP_SIZE = BLOCK_SIZE * 2;

// 地图中间
constexpr float PUBLIC_CAMP_X = CENTER_WIDTH / 2;
constexpr float PUBLIC_CAMP_Y = CENTER_HEIGHT / 2;

// 左下角
constexpr float CAMP_X = CAMP_SIZE / 2;
constexpr float CAMP_Y = CAMP_SIZE / 2;
// 右上角
constexpr float ENEMY_CAMP_X = CENTER_WIDTH - CAMP_SIZE / 2;
constexpr float ENEMY_CAMP_Y = CENTER_HEIGHT - CAMP_SIZE / 2;

//constexpr float CAMP_X = CAMP_SIZE / 2 + 100;
//constexpr float CAMP_Y = CAMP_SIZE / 2 + 100;

//constexpr float CAMP_X = CENTER_WIDTH - CAMP_SIZE / 2;
//constexpr float CAMP_Y = CENTER_HEIGHT - CAMP_SIZE / 2;
//constexpr float ENEMY_CAMP_X = CAMP_SIZE / 2;
//constexpr float ENEMY_CAMP_Y = CAMP_SIZE / 2;

//constexpr float CAMP_X = CAMP_SIZE / 2;
//constexpr float CAMP_Y = CENTER_HEIGHT - CAMP_SIZE / 2;
//constexpr float ENEMY_CAMP_X = CENTER_WIDTH - CAMP_SIZE / 2;
//constexpr float ENEMY_CAMP_Y = CAMP_SIZE / 2;

//constexpr float CAMP_X = CENTER_WIDTH - CAMP_SIZE / 2;
//constexpr float CAMP_Y = CAMP_SIZE / 2;
//constexpr float ENEMY_CAMP_X = CAMP_SIZE / 2;
//constexpr float ENEMY_CAMP_Y = CENTER_HEIGHT - CAMP_SIZE / 2;
