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
#include "DataStreamer.hpp"
#include <Logging.hpp>
#include "canard.h"
#include "Heartbeat_1_0.h"

//Strategy* stateMachine;
static THD_WORKING_AREA(waShellThread, SHELL_WA_SIZE);

void cyphalHeartBeatRoutine() {
    static CanardTransferID transfer_id = 0;
    uavcan_node_Mode_1_0 mode;
    mode.value = uavcan_node_Mode_1_0_OPERATIONAL;
    uavcan_node_Health_1_0 health;
    health.value = uavcan_node_Health_1_0_NOMINAL;
    uint32_t now = chVTGetSystemTime();
    const uavcan_node_Heartbeat_1_0 heartbeat = {
        .uptime = now,
        .health = health,
        .mode = mode,
    };

    size_t buf_size = uavcan_node_Heartbeat_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_;
    uint8_t buffer[uavcan_node_Heartbeat_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_];

    uavcan_node_Heartbeat_1_0_serialize_(&heartbeat, buffer, &buf_size);


    const CanardTransferMetadata metadata = {
        .priority = CanardPriorityNominal,
        .transfer_kind = CanardTransferKindMessage,
        .port_id = uavcan_node_Heartbeat_1_0_FIXED_PORT_ID_,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id = transfer_id,
    };
    transfer_id++;
    Board::Com::CANBus::send(&metadata, buf_size,  buffer);
}

int main() {
    halInit();
    halCommunityInit();
    chSysInit();

    Logging::init();
    Logging::println("[main] Starting up");
    shellInit();
    Board::init();
    chThdSleepMilliseconds(10);
//    ControlThread::instance()->start(NORMALPRIO + 1);
//    chThdSleepMilliseconds(10);
//    StrategyThread::instance()->start(NORMALPRIO + 2);
//    chThdSleepMilliseconds(10);
//    LidarThread::instance()->start(NORMALPRIO + 4);
//    chThdSleepMilliseconds(10);
//    AvoidanceThread::instance()->start(NORMALPRIO +5);
//    chThdSleepMilliseconds(10);
//    DataStreamer::instance()->start(NORMALPRIO);
//    chThdSleepMilliseconds(10);



    chThdCreateStatic(waShellThread, sizeof(waShellThread), NORMALPRIO,
                      shellThread, (void*)&shell_cfg);
    enum pliersState state;
    uint8_t i = 0;

    while (!chThdShouldTerminateX()) {
        Board::IO::toggleLED();
        cyphalHeartBeatRoutine();
        chThdSleepMilliseconds(1000);
    }

    Logging::println("[main] Shutting down");
}
