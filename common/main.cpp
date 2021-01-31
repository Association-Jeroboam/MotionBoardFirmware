#include <ch.h>
#include <hal.h>

#include <Logging.hpp>
#include "Board.hpp"
#include "BuildConf.hpp"



int main() {
    halInit();
    halCommunityInit();
    chSysInit();

    Logging::init();
    Logging::println("Starting up");
    Board::init();
    Board::IO::toggleLED();

    Board::IO::setMotorDutyCycle(Board::IO::LEFT_MOTOR, -0.5);
    chThdSleepMilliseconds(500);
    Board::IO::setMotorDutyCycle(Board::IO::LEFT_MOTOR, 0.);
    chThdSleepMilliseconds(500);
    Board::IO::setMotorDutyCycle(Board::IO::RIGHT_MOTOR, -0.5);
    chThdSleepMilliseconds(500);
    Board::IO::setMotorDutyCycle(Board::IO::RIGHT_MOTOR, 0.);
    int16_t leftEncCount  = Board::IO::getEncoderCount(Board::IO::LEFT_ENCODER);
    int16_t rightEncCount = Board::IO::getEncoderCount(Board::IO::RIGHT_ENCODER);
    Logging::println("left enc count %d", leftEncCount);
    Logging::println("right enc count %d", rightEncCount);
    chThdSleepMilliseconds(500);
    Board::IO::setMotorDutyCycle(Board::IO::LEFT_MOTOR, 0.5);
    chThdSleepMilliseconds(1000);
    Board::IO::setMotorDutyCycle(Board::IO::LEFT_MOTOR, 0.);
    chThdSleepMilliseconds(500);
    Board::IO::setMotorDutyCycle(Board::IO::RIGHT_MOTOR, 0.5);
    chThdSleepMilliseconds(1000);
    Board::IO::setMotorDutyCycle(Board::IO::RIGHT_MOTOR, 0.);
    leftEncCount  = Board::IO::getEncoderCount(Board::IO::LEFT_ENCODER);
    rightEncCount = Board::IO::getEncoderCount(Board::IO::RIGHT_ENCODER);
    Logging::println("left enc count %d", leftEncCount);
    Logging::println("right enc count %d", rightEncCount);

    while (!chThdShouldTerminateX()) {
        chThdSleepMilliseconds(500);
        Board::IO::toggleLED();

    }

    Logging::println("Shutting down");
}
