#include "MotionBoard.hpp"
#include "Actuators.hpp"
#include "ch.hpp"

void Actuators::FrontPliers::open(enum FrontPliers::ID id) {
    enum pliersID pliersID;
    switch (id) {
        case FAR_LEFT:
            pliersID = PLIERS_FRONT_FAR_LEFT;
            break;
        case LEFT:
            pliersID = PLIERS_FRONT_LEFT;
            break;
        case RIGHT:
            pliersID = PLIERS_FRONT_RIGHT;
            break;
        case FAR_RIGHT:
            pliersID = PLIERS_FRONT_FAR_RIGHT;
            break;
    }
    canFrame_t frame = {
        .ID = CAN_PLIERS_ID,
        .len = CAN_PLIERS_LEN,
    };
    frame.data.pliersData.plierID = pliersID;
    frame.data.pliersData.state = PLIERS_IDLE;
    Board::Com::CANBus::send(frame);
    chThdSleepMilliseconds(10);
}

void Actuators::FrontPliers::close(enum FrontPliers::ID id) {
    enum pliersID pliersID;
    switch (id) {
        case FAR_LEFT:
            pliersID = PLIERS_FRONT_FAR_LEFT;
            break;
        case LEFT:
            pliersID = PLIERS_FRONT_LEFT;
            break;
        case RIGHT:
            pliersID = PLIERS_FRONT_RIGHT;
            break;
        case FAR_RIGHT:
            pliersID = PLIERS_FRONT_FAR_RIGHT;
            break;
    }
    canFrame_t frame = {
        .ID = CAN_PLIERS_ID,
        .len = CAN_PLIERS_LEN,
    };
    frame.data.pliersData.plierID = pliersID;
    frame.data.pliersData.state = PLIERS_ACTIVATED;
    Board::Com::CANBus::send(frame);
    chThdSleepMilliseconds(10);
}

void Actuators::BackPliers::open(enum BackPliers::ID id) {
    enum pliersID pliersID;
    switch (id) {
        case FAR_RIGHT:
            pliersID = PLIERS_REAR_FAR_RIGHT;
            break;
        case RIGHT:
            pliersID = PLIERS_REAR_RIGHT;
            break;
        case MIDDLE:
            pliersID = PLIERS_REAR_MIDDLE;
            break;
        case LEFT:
            pliersID = PLIERS_REAR_LEFT;
            break;
        case FAR_LEFT:
            pliersID = PLIERS_REAR_FAR_LEFT;
            break;
    }
    canFrame_t frame = {
        .ID = CAN_PLIERS_ID,
        .len = CAN_PLIERS_LEN,
    };
    frame.data.pliersData.plierID = pliersID;
    frame.data.pliersData.state = PLIERS_IDLE;
    Board::Com::CANBus::send(frame);
    chThdSleepMilliseconds(10);
}

void Actuators::BackPliers::close(enum BackPliers::ID id) {
    enum pliersID pliersID;
    switch (id) {
        case FAR_RIGHT:
            pliersID = PLIERS_REAR_FAR_RIGHT;
            break;
        case RIGHT:
            pliersID = PLIERS_REAR_RIGHT;
            break;
        case MIDDLE:
            pliersID = PLIERS_REAR_MIDDLE;
            break;
        case LEFT:
            pliersID = PLIERS_REAR_LEFT;
            break;
        case FAR_LEFT:
            pliersID = PLIERS_REAR_FAR_LEFT;
            break;
    }
    canFrame_t frame = {
        .ID = CAN_PLIERS_ID,
        .len = CAN_PLIERS_LEN,
    };
    frame.data.pliersData.plierID = pliersID;
    frame.data.pliersData.state = PLIERS_ACTIVATED;
    Board::Com::CANBus::send(frame);
    chThdSleepMilliseconds(10);
}

void Actuators::BackPliers::engage() {
    canFrame_t frame = {
        .ID = CAN_PLIERS_BLOCK_ID,
        .len = CAN_PLIERS_BLOCK_LEN,
    };
    frame.data.pliersBlockData.state = 1;
    Board::Com::CANBus::send(frame);
    chThdSleepMilliseconds(10);
}

void Actuators::BackPliers::disengage() {
    canFrame_t frame = {
        .ID = CAN_PLIERS_BLOCK_ID,
        .len = CAN_PLIERS_BLOCK_LEN,
    };
    frame.data.pliersBlockData.state = 0;
    Board::Com::CANBus::send(frame);
    chThdSleepMilliseconds(10);
}

void Actuators::Arms::activate(enum ID id) {
    enum armID armID;
    switch (id) {
        case LEFT:
            armID = ARM_LEFT;
            break;
        case RIGHT:
            armID = ARM_RIGHT;
            break;

    }
    canFrame_t frame = {
        .ID = CAN_ARMS_ID,
        .len = CAN_ARMS_LEN,
    };
    frame.data.armData.armID = armID;
    frame.data.armData.state = PLIERS_ACTIVATED;
    Board::Com::CANBus::send(frame);
    chThdSleepMilliseconds(10);
}

void Actuators::Arms::deactivate(enum ID id) {
    enum armID armID;
    switch (id) {
        case LEFT:
            armID = ARM_LEFT;
            break;
        case RIGHT:
            armID = ARM_RIGHT;
            break;

    }
    canFrame_t frame = {
        .ID = CAN_ARMS_ID,
        .len = CAN_ARMS_LEN,
    };
    frame.data.armData.armID = armID;
    frame.data.armData.state = PLIERS_IDLE;
    Board::Com::CANBus::send(frame);
    chThdSleepMilliseconds(10);
}
