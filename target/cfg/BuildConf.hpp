#pragma once

#include "Board.hpp"

#define LOGGING_DRIVER   SD4
#define MOTOR_PWM_DRIVER PWMD1 //might change

#define LED_LINE         LINE_LED

namespace Board {
    namespace IO {
        enum motor : uint8_t {
            LEFT_MOTOR  = 1,
            RIGHT_MOTOR = 0,
        };
    }
}

namespace Board {
    namespace IO {
        enum encoder : uint8_t {
            LEFT_ENCODER  = 0,
            RIGHT_ENCODER = 1,
        };
    }
}
