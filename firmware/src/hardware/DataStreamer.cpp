#include "DataStreamer.hpp"
#include "Logging.hpp"
#include "MotionBoard.hpp"
#include <new>

enum DataStreamEvent{
    BoardEvent = 1 <<0,
};

using namespace chibios_rt;

DataStreamer DataStreamer::s_instance;

DataStreamer* DataStreamer::instance() {
    return &s_instance;
}

void DataStreamer::main() {
    setName("Data Streamer");
    Logging::println("Starting Data Stream, disabling logging");
    chThdSleepMilliseconds(20);
    Logging::setDisablePrint(true);
    Board::Events::eventRegister(this, BoardEvent);

    while (!shouldTerminate()) {

        eventmask_t event = waitOneEvent(BoardEvent);
        if (event & BoardEvent) {
            eventflags_t flags = getAndClearFlags();
            if (flags & Board::Events::RUN_MOTOR_CONTROL) {
                sendData();
            }
        }
    }
    chThdSleepMilliseconds(20);
    Logging::setDisablePrint(false);
    Logging::println("Data Stream disabled, enabling logging");
}

void DataStreamer::sendData() {
    m_data.timestamp = m_packetCounter;
    Logging::write((uint8_t*)&m_data, sizeof(DataStreamType));
    m_packetCounter++;
}

DataStreamer::DataStreamer() : BaseStaticThread<DATA_STREAMER_WA>(), EventListener() {
    m_data.synchro  = 0xDEADBEEF;
    m_packetCounter = 0;
}

#define ENTRY(entry)               \
    case entry##Enum: {            \
        m_data.data.entry = value; \
        break;                     \
    }

void DataStreamer::setEntry(enum dataEntryEnum entry, float value) {
    switch (entry) {
        DATA_ENTRY_LIST
    }
}
#undef ENTRY
