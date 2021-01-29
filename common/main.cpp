#include <ch.h>
#include <hal.h>

#include <Logging.hpp>
#include "Board.hpp"
#include "hal_pal.h"

int main() {
    halInit();
    chSysInit();

    Board::Com::initDrivers();

    Logging::println("Starting up");

    while (!chThdShouldTerminateX()) {
        chThdSleepMilliseconds(500);
        palWriteLine(Board::IO::getLedLine(), PAL_HIGH);
        chThdSleepMilliseconds(500);
        palWriteLine(Board::IO::getLedLine(), PAL_LOW);
    }

    Logging::println("Shutting down");
}
