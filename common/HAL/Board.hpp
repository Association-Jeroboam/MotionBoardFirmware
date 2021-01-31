#pragma once

#include "inttypes.h"

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
}
