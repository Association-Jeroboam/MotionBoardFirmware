#pragma once

#include "inttypes.h"

namespace Board {
    namespace IO {

        enum motor : uint16_t;

        void initDrivers();
        void deinitPWM();
        void setMotorDutyCycle(enum motor motor, uint16_t duty_cycle);
        int16_t getLeftEncoderCount();
        int16_t getRightEncoderCount();

        void toggleLED();

    }

    namespace Com {
        void initDrivers();
    }
}
