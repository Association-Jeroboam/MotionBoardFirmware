#pragma once

#include "Peripherals.hpp"
#include "ch.hpp"
#include "inttypes.h"

namespace Board {
void init();

namespace IO {

void initDrivers();

void deinitPWM();

void setMotorDutyCycle(Peripherals::Motor motor, float duty_cycle);

int16_t getEncoderCount(Peripherals::Encoder encoder);

void toggleLED();

} // namespace IO

namespace Com {
void initDrivers();
}

namespace Events {

enum event {
    RUN_MOTOR_CONTROL = 1 << 0,
    SEND_STREAM       = 1 << 1,
};

void eventRegister(chibios_rt::EventListener* elp, enum event event);

void startControlLoop(uint16_t frequency);
} // namespace Events
} // namespace Board
