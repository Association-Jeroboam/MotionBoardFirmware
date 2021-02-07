#pragma once

#include "board.h"

// Communications
#define LOGGING_DRIVER           SD2

// Motors

namespace Board {
    namespace IO {
        enum motor : uint16_t{
            LEFT_MOTOR =  1,
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

constexpr uint16_t ENCODER_TICK_PER_TURN = 48;

#define GEAR_RATIO_NUM           5622144. // 22*22*22*22*24
#define GEAR_RATIO_DEN           120000.  // 12*10*10*10*10
constexpr float GEAR_RATIO     = ((GEAR_RATIO_NUM)/(GEAR_RATIO_DEN));
constexpr float GEAR_RATIO_INV = ((GEAR_RATIO_DEN)/(GEAR_RATIO_NUM));

constexpr float WHEEL_LEFT_RADIUS  = 0.064; //[m] Theorical 0.064
constexpr float WHEEL_RIGHT_RADIUS = 0.064; //[m] Theorical 0.064
constexpr float WHEEL_BASE         = 0.186; //[m] Theorical 0.186

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

// GPIO

#define LED_LINE         LINE_ARD_D13

#define LOGGING_DRIVER   SD2
#define MOTOR_PWM_DRIVER PWMD1

#define LED_LINE         LINE_ARD_D13
