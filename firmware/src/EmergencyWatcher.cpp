#include "EmergencyWatcher.hpp"
#include "MotionBoard.hpp"
#include "Logging.hpp"
#include "EmergencyState_0_1.h"

using namespace Board::Events;

enum EmgcyWatcherEvents {
    BoardEvent = 1 << 0,

};

EmergencyWatcher::EmergencyWatcher() :
chibios_rt::BaseStaticThread<EMGCY_WATCHER_WA>(),
m_listener(),
m_emgcy_state(false)
{

}

void EmergencyWatcher::main() {
    setName("Emgcy Watcher");
    m_emgcy_state = Board::IO::getEmgcyState();
    Board::Events::eventRegister(&m_listener, BoardEvent);

    while (!shouldTerminate()) {
        eventmask_t event =waitOneEvent(BoardEvent);
        if(event & BoardEvent) {
            eventflags_t flags = m_listener.getAndClearFlags();
            if(flags & RUN_MOTOR_CONTROL) {
                sendEmgcyState();
            }

            if (flags & EMERGENCY_STOP) {
                Logging::println("Emergency stop!");
                m_emgcy_state = true;
            }
    
            if(flags & EMERGENCY_CLEARED){
                Logging::println("Emergency cleared!");
                m_emgcy_state = false;
            }
        }
    }
}

void EmergencyWatcher::sendEmgcyState() {
    static CanardTransferID transfer_id = 0;
    jeroboam_datatypes_actuators_common_EmergencyState_0_1 emgcyState;
    emgcyState.emergency.value = m_emgcy_state;


    size_t buf_size = jeroboam_datatypes_actuators_common_EmergencyState_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_;
    uint8_t buffer[jeroboam_datatypes_actuators_common_EmergencyState_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_];

    jeroboam_datatypes_actuators_common_EmergencyState_0_1_serialize_(&emgcyState, buffer, &buf_size);


    const CanardTransferMetadata metadata = {
        .priority = CanardPriorityNominal,
        .transfer_kind = CanardTransferKindMessage,
        .port_id = EMERGENCY_STATE_ID,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id = transfer_id,
    };
    transfer_id++;
    Board::Com::CANBus::send(&metadata, buf_size,  buffer);
}
