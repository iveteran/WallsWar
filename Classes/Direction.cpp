#include "Direction.h"

Direction reverseDirection(Direction dir) {
    if (dir == Direction::RIGHT || dir == Direction::DOWN) {
        dir = (Direction)((int)dir - 2);
    } else if (dir == Direction::LEFT || dir == Direction::UP) {
        dir = (Direction)((int)dir + 2);
    }
    return dir;
}
