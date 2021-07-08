#include "Switchers.hpp"
#include "../MotionBoard.hpp"

bool Switchers::getSide() {
    return Board::IO::getSide();
}

uint8_t Switchers::getStrategy() {
    return Board::IO::getStrategy();
}
