#include <ch.h>
#include <hal.h>

#include <Logging.hpp>
#include "Board.hpp"
#include "BuildConf.hpp"



int main() {
    halInit();
    chSysInit();


    Logging::init();
    Logging::println("Starting up");
    Board::Com::initDrivers();
    Board::IO::initDrivers();
    Board::IO::toggleLED();
    //motor test routine
    Board::IO::setMotorDutyCycle(Board::IO::LEFT_MOTOR, 2500);
    chThdSleepMilliseconds(500);
    Board::IO::setMotorDutyCycle(Board::IO::LEFT_MOTOR, 5000);
    chThdSleepMilliseconds(1000);
    Board::IO::setMotorDutyCycle(Board::IO::LEFT_MOTOR, 7500);
    chThdSleepMilliseconds(500);
    Board::IO::setMotorDutyCycle(Board::IO::LEFT_MOTOR, 5000);
    chThdSleepMilliseconds(1000);
    Board::IO::setMotorDutyCycle(Board::IO::RIGHT_MOTOR, 2500);
    chThdSleepMilliseconds(500);
    Board::IO::setMotorDutyCycle(Board::IO::RIGHT_MOTOR, 5000);
    chThdSleepMilliseconds(1000);
    Board::IO::setMotorDutyCycle(Board::IO::RIGHT_MOTOR, 7500);
    chThdSleepMilliseconds(500);
    Board::IO::setMotorDutyCycle(Board::IO::RIGHT_MOTOR, 5000);

    while (!chThdShouldTerminateX()) {
        chThdSleepMilliseconds(500);
        Board::IO::toggleLED();

    }

    Logging::println("Shutting down");
}
