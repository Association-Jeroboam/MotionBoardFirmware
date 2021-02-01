#pragma once

#include "inttypes.h"
#include "ch.hpp"

namespace Board {
    void init();

    namespace IO {

        enum motor   : uint8_t;
        enum encoder : uint8_t;

        void initDrivers();
        void deinitPWM();
        void setMotorDutyCycle(enum motor motor, float duty_cycle);
        int16_t getEncoderCount(enum encoder encoder);

        void toggleLED();

    }

    namespace Com {
        void initDrivers();
    }

    namespace Events {

        enum event {
            RUN_MOTOR_CONTROL = 1 << 0,
        };

        void eventRegister(chibios_rt::EventListener *elp, enum event event);

        void startMotorControlLoop(uint16_t frequency);
    }
}
