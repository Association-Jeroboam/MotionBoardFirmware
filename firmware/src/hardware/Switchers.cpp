#include "Switchers.hpp"
#include "../MotionBoard.hpp"

bool Switchers::getSide() {
    return Board::IO::getSide();
}

uint8_t Switchers::getStrategy() {
    return Board::IO::getStrategy();
}

void Switchers::riseFlag(){
//    canFrame_t frame = {
//        .ID  = CAN_PLIERS_ID,
//        .len = CAN_PLIERS_LEN,
//    };
//
//    frame.data.pliersData.plierID = PLIERS_FRONT_FAR_LEFT;
//    frame.data.pliersData.state   = PLIERS_CLOSE;
//    Board::Com::CANBus::send(frame);
}
