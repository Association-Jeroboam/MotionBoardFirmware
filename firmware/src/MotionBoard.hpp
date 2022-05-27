#pragma once

#include "CanListener.hpp"
#include "CanProtocol.hpp"
#include "Peripherals.hpp"
#include "ch.hpp"
#include "inttypes.h"
#include "canard.h"

namespace Board {
void init();


namespace IO {

void    initDrivers();
void    initPWM();
void    initEncoders();
void    initTimers();
void    initGPIO();
bool    getEmgcyState();

void setBrake(Peripherals::Motor motor, bool brake);

void deinitPWM();

void setMotorDutyCycle(Peripherals::Motor motor, float duty_cycle);

int16_t getEncoderCount(Peripherals::Encoder encoder);
float   getMotorSpeed(Peripherals::Motor motor);

void toggleLED();

} // namespace IO

namespace Com {
void initDrivers();
namespace CANBus {
void init();
bool send(const CanardTransferMetadata* const metadata,
          const size_t                        payload_size,
          const void* const                   payload);
void registerCanMsg(CanListener *listener,
                    CanardTransferKind transfer_kind,
                    CanardPortID port_id,
                    size_t extent);
} // namespace CANBus

} // namespace Com

namespace Events {

enum eventFlags {
    RUN_MOTOR_CONTROL = 1 << 0,
    SEND_STREAM       = 1 << 1,
    EMERGENCY_STOP    = 1 << 2,
    EMERGENCY_CLEARED = 1 << 3,
};

void eventRegister(chibios_rt::EventListener* elp, eventmask_t event);

void startControlLoop(uint16_t frequency);
} // namespace Events
} // namespace Board
