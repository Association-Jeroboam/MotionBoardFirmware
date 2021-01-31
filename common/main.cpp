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

    Board::IO::setMotorDutyCycle(Board::IO::LEFT_MOTOR, 3500);
    Board::IO::setMotorDutyCycle(Board::IO::RIGHT_MOTOR, 3500);

    while (!chThdShouldTerminateX()) {
        chThdSleepMilliseconds(500);
        Board::IO::toggleLED();
        int16_t leftEncoderCount = Board::IO::getLeftEncoderCount();
        int16_t rightEncoderCount = Board::IO::getLeftEncoderCount();
        Logging::println("left encoder cnt : %d", leftEncoderCount);
        Logging::println("right encoder cnt: %d", rightEncoderCount);

    }

    Logging::println("Shutting down");
}
