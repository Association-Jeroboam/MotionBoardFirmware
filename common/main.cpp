#include <ch.h>
#include <hal.h>

#include <Logging.hpp>
#include "Board.hpp"

int main() {
    halInit();
    chSysInit();


    Logging::init();
    Logging::println("Starting up");
    Board::Com::initDrivers();
    Board::IO::initDrivers();

    while (!chThdShouldTerminateX()) {
        chThdSleepMilliseconds(500);
        Board::IO::toggleLED();

    }

    Logging::println("Shutting down");
}
