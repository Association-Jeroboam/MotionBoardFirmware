#include <ch.hpp>
#include <hal.h>
#include <shell.h>

#include "AvoidanceThread.hpp"
#include "ControlThread.hpp"
#include "LidarThread.hpp"
#include "MotionBoard.hpp"
#include "MotionBoardShell.hpp"
#include "Strategy/Events.hpp"
#include "StrategyThread.hpp"
#include <Logging.hpp>

Strategy* stateMachine;
static THD_WORKING_AREA(waShellThread, SHELL_WA_SIZE);

int main() {
    halInit();
    halCommunityInit();
    chSysInit();

    Logging::init();
    Logging::println("[main] Starting up");
    shellInit();
    Board::init();
    chThdSleepMilliseconds(10);
    ControlThread::instance()->start(NORMALPRIO + 1);
    chThdSleepMilliseconds(10);
    StrategyThread::instance()->start(NORMALPRIO + 2);
    chThdSleepMilliseconds(10);
    LidarThread::instance()->start(NORMALPRIO + 4);
    chThdSleepMilliseconds(10);
    AvoidanceThread::instance()->start(NORMALPRIO +5);
    chThdSleepMilliseconds(10);

    chThdCreateStatic(waShellThread, sizeof(waShellThread), NORMALPRIO,
                      shellThread, (void*)&shell_cfg);
    while (!chThdShouldTerminateX()) {
        Board::IO::toggleLED();
        chThdSleepMilliseconds(2000);
    }

    Logging::println("[main] Shutting down");
}
