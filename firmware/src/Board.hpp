#pragma once

#include "inttypes.h"
#include "ch.hpp"
#include "Peripherals.hpp"

namespace Board {
    void init();

    namespace IO {

        void initDrivers();

        void deinitPWM();

        void setMotorDutyCycle(Peripherals::Motor motor, float duty_cycle);

        int16_t getEncoderCount(Peripherals::Encoder encoder);

        void toggleLED();

    }

    namespace Com {
        void initDrivers();
    }

    namespace Events {

        enum event {
            RUN_MOTOR_CONTROL = 1 << 0,
            SEND_STREAM       = 1 << 1,
        };

        void eventRegister(chibios_rt::EventListener *elp, enum event event);

        void startControlLoop(uint16_t frequency);
    }
}
