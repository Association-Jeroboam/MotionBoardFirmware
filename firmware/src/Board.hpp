#pragma once

#include "Peripherals.hpp"
#include "ch.hpp"
#include "inttypes.h"
#include "CanProtocol.hpp"
#include "CanListener.hpp"

namespace Board {
void init();

namespace IO {

void initDrivers();
void initPWM();
void initEncoders();
void initTimers();
void initGPIO();
bool getSide();
uint8_t getStrategy();

void deinitPWM();

void setMotorDutyCycle(Peripherals::Motor motor, float duty_cycle);

int16_t getEncoderCount(Peripherals::Encoder encoder);

void toggleLED();

} // namespace IO

namespace Com {
void initDrivers();
namespace CANBus{
    void init();
    bool send(canFrame_t canData);
    void registerListener(CanListener * listener);
} //namespace CanBus

namespace Lidar {
    void init();
} // namespace Lidar
} // namespace Com

namespace Events {

enum eventFlags {
    RUN_MOTOR_CONTROL = 1 << 0,
    SEND_STREAM       = 1 << 1,
    START_MATCH       = 1 << 2,
    EMERGENCY_STOP    = 1 << 3,
    EMERGENCY_CLEARED = 1 << 4,
};

void eventRegister(chibios_rt::EventListener* elp, eventmask_t event);

void startControlLoop(uint16_t frequency);
void startStartMatchTimer(uint16_t interval_ms);
} // namespace Events
} // namespace Board
