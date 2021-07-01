#include <ch.hpp>
#include <hal.h>
#include <shell.h>

#include "Board.hpp"
#include "ControlThread.hpp"
#include "Strategy/Events.hpp"
#include "Shell.hpp"
#include "StrategyThread.hpp"
#include <Logging.hpp>
#include "LidarThread.hpp"
#include "AvoidanceThread.hpp"

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
    ControlThread::instance()->start(NORMALPRIO+1);
    chThdSleepMilliseconds(10);
//    StrategyThread::instance()->start(NORMALPRIO+2);
//    chThdSleepMilliseconds(10);
//    LidarThread::instance()->start(NORMALPRIO+3);
//    chThdSleepMilliseconds(10);
//    AvoidanceThread::instance()->start(NORMALPRIO - 1);
//    chThdSleepMilliseconds(10);

    chThdCreateStatic(waShellThread, sizeof(waShellThread), NORMALPRIO,
                      shellThread, (void*)&shell_cfg);
    enum pliersState state = PLIERS_OPEN;
    while (!chThdShouldTerminateX()) {
        canFrame_t frame;
        frame.ID = CAN_PLIERS_ID;
        frame.data.pliersData = {
            .plierID = PLIERS_REAR_MIDDLE,
            .state = state,
        };
        frame.len = CAN_PLIERS_LEN;

        Board::Com::CANBus::send(frame);

        if (state == PLIERS_OPEN) {
            state = PLIERS_CLOSE;
        } else {
            state = PLIERS_OPEN;
        }
//        Logging::println("[CAN] sent pliers %u state %u", PLIERS_REAR_MIDDLE, state);
        Board::IO::toggleLED();
        bool side = Board::IO::getSide();
        uint8_t strategy = Board::IO::getStrategy();
        Logging::println("side %u strategy %u", side, strategy);
        chThdSleepMilliseconds(2000);

    }

    Logging::println("[main] Shutting down");
}
