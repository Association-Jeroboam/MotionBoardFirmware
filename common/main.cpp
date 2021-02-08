#include <ch.hpp>
#include <hal.h>
#include <shell.h>

#include "Board.hpp"
#include "MotorControlLoop.hpp"
#include "Shell.hpp"
#include <Logging.hpp>

MotorControlLoop motorControlLoop;

int main() {
    halInit();
    halCommunityInit();
    chSysInit();

    Logging::init();
    Logging::println("Starting up");
    shellInit();
    Board::init();

    motorControlLoop.start(HIGHPRIO);
    chThdSleepMilliseconds(20);

    chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                        "shell", NORMALPRIO + 1,
                        shellThread, (void *) &shell_cfg);

    while (!chThdShouldTerminateX()) {
        chThdSleepMilliseconds(20);
    }

    Logging::println("Shutting down");
}
