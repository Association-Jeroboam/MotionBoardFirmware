#pragma once

#include "ch.hpp"

constexpr uint16_t DATA_STREAMER_WA = 256;

#define DATA_ENTRY_LIST              \
ENTRY(leftWheelSpeed)                \
ENTRY(leftWheelSpeedSetpoint)        \
ENTRY(rightWheelSpeed)               \
ENTRY(rightWheelSpeedSetpoint)

#define ENTRY(entry) entry##Enum,
enum dataEntryEnum : uint8_t {
    DATA_ENTRY_LIST
};
#undef ENTRY

#define ENTRY(entry) float entry;
struct dataEntry {
    DATA_ENTRY_LIST
} __attribute__((packed));
#undef ENTRY

typedef struct {
    uint32_t  synchro;
    uint32_t  timestamp;
    dataEntry data;
}  __attribute__((packed)) DataStreamType;

class DataStreamer : public chibios_rt::BaseStaticThread<DATA_STREAMER_WA>,
                     public chibios_rt::EventListener {

public:
    static DataStreamer *instance();

    void main() override;

    void setEntry(enum dataEntryEnum entry, float value);

private:
    explicit DataStreamer();

    void sendData();

    uint32_t m_packetCounter;
    volatile DataStreamType m_data;

};
