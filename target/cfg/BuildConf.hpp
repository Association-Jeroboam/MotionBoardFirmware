#pragma once

#define LOGGING_DRIVER   SD4
#define MOTOR_PWM_DRIVER PWMD1 //might change

#define LED_LINE         LINE_LED

namespace Board {
    namespace IO {
        enum motor : uint16_t{
            LEFT_MOTOR =  0,
            RIGHT_MOTOR = 1,
        };
    }
}
