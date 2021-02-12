#pragma once

#include "board.h"
#include "Board.hpp"

// Communications
#define LOGGING_DRIVER           SD2
#define SHELL_DRIVER             LOGGING_DRIVER

// Motors

namespace Board {
    namespace IO {
        enum motor : uint8_t {
            LEFT_MOTOR  = 1,
            RIGHT_MOTOR = 0,
        };
    }
}

#define MOTOR_PWM_DRIVER         PWMD1

#define MOTOR_LEFT_P_CHAN_LINE   PAL_LINE(GPIOA, 9U)
#define MOTOR_LEFT_N_CHAN_LINE   PAL_LINE(GPIOB, 0U)
#define MOTOR_RIGHT_P_CHAN_LINE  PAL_LINE(GPIOA, 8U)
#define MOTOR_RIGHT_N_CHAN_LINE  PAL_LINE(GPIOA, 7U)

// Encoders

namespace Board {
    namespace IO {
        enum encoder : uint8_t {
            LEFT_ENCODER  = 0,
            RIGHT_ENCODER = 1,
        };
    }
}

#define LEFT_ENCODER_DRIVER      QEID2
#define RIGHT_ENCODER_DRIVER     QEID3

#define ENCODER_LEFT_CHAN1_LINE  PAL_LINE(GPIOA, 0U)
#define ENCODER_LEFT_CHAN2_LINE  PAL_LINE(GPIOA, 1U)
#define ENCODER_RIGHT_CHAN1_LINE PAL_LINE(GPIOA, 4U)
#define ENCODER_RIGHT_CHAN2_LINE PAL_LINE(GPIOA, 6U)

#define ENCODER_LEFT_CHAN1_PIN_MODE   1
#define ENCODER_LEFT_CHAN2_PIN_MODE   1
#define ENCODER_RIGHT_CHAN1_PIN_MODE  2
#define ENCODER_RIGHT_CHAN2_PIN_MODE  2

// Timers

#define MOTOR_CONTROL_LOOP_TIMER      GPTD7

// GPIO

#define LED_LINE         LINE_ARD_D13
