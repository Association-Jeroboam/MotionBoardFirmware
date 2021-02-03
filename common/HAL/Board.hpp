#pragma once

#include "inttypes.h"

namespace Board {
    namespace IO {

        enum motor : uint16_t {
            LEFT_MOTOR  = 0,
            RIGHT_MOTOR = 1,
        };

        void initDrivers();
        void deinitPWM();
        void setMotorDutyCycle(enum motor motor, uint16_t duty_cycle);

        void toggleLED();

    }

    namespace Com {
        void initDrivers();
    }
}
