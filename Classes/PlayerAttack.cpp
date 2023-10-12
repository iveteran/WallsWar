#include "Player.h"

// 投掷炸弹，可越过障碍物，除了在命中点伤害敌方，也可在命中点的周围(2*2)散发弹片伤害敌方玩家
void Player::dropBomb() {
    if (isBeControl() || _bombs == 0) {
        return;
    }
    // TODO
    _bombs--;
}

// 投掷烟雾弹，在自己周边两层大小的区域(3*3)产生烟雾遮罩，让敌方不知自己和队友的行动方向
void Player::dropSmokeBomb() {
    if (isBeControl() || _smokeBombs == 0) {
        return;
    }
    // TODO
    _smokeBombs--;
}

// 投掷照明弹，在自己视野外的地方照亮敌方玩家并在小地图显示，落点周边区域大小3*3
void Player::dropFlashBomb() {
    if (isBeControl() || _flashBombs == 0) {
        return;
    }
    // TODO
    _flashBombs--;
}

// 投掷燃烧弹
void Player::dropFireBomb() {
    if (isBeControl() || _fireBombs == 0) {
        return;
    }
    // TODO
    _fireBombs--;
}
