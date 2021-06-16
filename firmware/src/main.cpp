#include <ch.hpp>
#include <hal.h>
#include <shell.h>

#include "Board.hpp"
#include "ControlThread.hpp"
#include "Strategy/Events.hpp"
#include "Shell.hpp"
#include "StrategyThread.hpp"
#include <Logging.hpp>

Strategy* stateMachine;

int main() {
    halInit();
    halCommunityInit();
    chSysInit();

    Logging::init();
    Logging::println("[main] Starting up");
    shellInit();
    Board::init();

    ControlThread::instance()->start(NORMALPRIO+1);
    chThdSleepMilliseconds(10);
    StrategyThread::instance()->start(NORMALPRIO+2);
    chThdYield();
    chThdSleepMilliseconds(10);

    chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                        "shell", NORMALPRIO + 1,
                        shellThread, (void*)&shell_cfg);

    while (!chThdShouldTerminateX()) {
        chThdSleepMilliseconds(20);
    }

    Logging::println("[main] Shutting down");
}
