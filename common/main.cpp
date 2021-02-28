#include <ch.hpp>
#include <hal.h>
#include <shell.h>

#include "Board.hpp"
#include "Shell.hpp"
#include <Logging.hpp>
#include "Control.hpp"


int main() {
    halInit();
    halCommunityInit();
    chSysInit();

    Logging::init();
    Logging::println("Starting up");
    shellInit();
    Board::init();

    Control::instance()->start(HIGHPRIO);
    chThdSleepMilliseconds(20);

    chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                        "shell", NORMALPRIO + 1,
                        shellThread, (void *) &shell_cfg);

    while (!chThdShouldTerminateX()) {
        chThdSleepMilliseconds(20);
    }

    Logging::println("Shutting down");
}
