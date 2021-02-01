#include <ch.hpp>
#include <hal.h>

#include <Logging.hpp>
#include "Board.hpp"
#include "MotorControlLoop.hpp"

int main() {
    halInit();
    halCommunityInit();
    chSysInit();

    Logging::init();
    Logging::println("Starting up");
    Board::init();
    MotorControlLoop motorControlLoop;
    motorControlLoop.start(HIGHPRIO);

    while (!chThdShouldTerminateX()) {
        chThdSleepMilliseconds(20);
    }

    Logging::println("Shutting down");
}
