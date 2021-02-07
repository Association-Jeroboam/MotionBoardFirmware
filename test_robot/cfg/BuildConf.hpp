#pragma once

#define LOGGING_DRIVER   SD2
#define MOTOR_PWM_DRIVER PWMD1

#define LED_LINE         LINE_ARD_D13

namespace Board {
    namespace IO {
        enum motor : uint16_t{
            LEFT_MOTOR =  1,
            RIGHT_MOTOR = 0,
        };
    }
}
