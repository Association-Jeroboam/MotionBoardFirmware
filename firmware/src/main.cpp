#include <ch.hpp>
#include <hal.h>
#include <shell.h>

#include "ControlThread.hpp"
#include "MotionBoard.hpp"
#include "MotionBoardShell.hpp"
#include "DataStreamer.hpp"
#include <Logging.hpp>
#include "canard.h"
#include "Heartbeat_1_0.h"
#include "cartesian/Pose_0_1.h"
#include "EmergencyWatcher.hpp"

static THD_WORKING_AREA(waShellThread, SHELL_WA_SIZE);

EmergencyWatcher emergencyWatcher;

void cyphalHeartBeatRoutine() {
    static CanardTransferID transfer_id = 0;
    static uint16_t MSB = 0;
    static uint32_t before = 0;
    uint32_t now = chVTGetSystemTime();
    if(now <= before) {
        MSB++;
    }
    const uavcan_node_Heartbeat_1_0 heartbeat = {
        .uptime = TIME_I2S(now | (MSB << 16)),
        .health = {
            .value = uavcan_node_Health_1_0_NOMINAL
        },
        .mode = {
            .value = uavcan_node_Mode_1_0_OPERATIONAL,
        },
        .vendor_specific_status_code = 42,
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
    before = now;
}


int main() {
    halInit();
    halCommunityInit();
    chSysInit();

    Logging::init();
    Logging::println("[main] Starting up");
    shellInit();
    Board::init();
    emergencyWatcher.start(NORMALPRIO);
    chThdSleepMilliseconds(10);
    ControlThread::instance()->start(NORMALPRIO + 1);
    chThdSleepMilliseconds(10);
    DataStreamer::instance()->start(NORMALPRIO);
    chThdSleepMilliseconds(10);



    chThdCreateStatic(waShellThread, sizeof(waShellThread), NORMALPRIO,
                      shellThread, (void*)&shell_cfg);

    while (!chThdShouldTerminateX()) {
        Board::IO::toggleLED();
        cyphalHeartBeatRoutine();
        chThdSleepMilliseconds(1000);
    }

    Logging::println("[main] Shutting down");
}
