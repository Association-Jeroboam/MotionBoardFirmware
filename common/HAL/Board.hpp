#pragma once

#include "inttypes.h"

namespace Board {
    namespace IO {

        enum motor : uint16_t;

        void initDrivers();
        void deinitPWM();
        void setMotorDutyCycle(enum motor motor, uint16_t duty_cycle);

        void toggleLED();

    }

    namespace Com {
        void initDrivers();
    }
}
