#pragma once

#include "MotionBoard.hpp"
#include "hal.h"
#define LOGGING_DRIVER SD2
#define SHELL_DRIVER LOGGING_DRIVER
#define MOTOR_PWM_DRIVER PWMD3  //right for now

#define LOGGING_TX_PIN PAL_LINE(GPIOA, 2U)
#define LOGGING_TX_PIN_MODE PAL_MODE_ALTERNATE(7)
#define LOGGING_RX_PIN PAL_LINE(GPIOA, 3U)
#define LOGGING_RX_PIN_MODE PAL_MODE_ALTERNATE(7)

#define CAN_DRIVER CAND1
#define CAN_TX_PIN      PAL_LINE(GPIOA, 11U)
#define CAN_TX_PIN_MODE PAL_MODE_ALTERNATE(9)
#define CAN_RX_PIN      PAL_LINE(GPIOA, 12U)
#define CAN_RX_PIN_MODE PAL_MODE_ALTERNATE(9)

CANConfig const canConfig = {
    .NBTP  = 0x2070C01,
    .DBTP  = 0,
    .CCCR  = 0,
    .TEST  = 0,
    .RXGFC = 0,
};

constexpr uint32_t ICU_FREQUENCY = 100000;
constexpr float TACHO_TPULE_RC_R = 47000; // 47kOhm
constexpr float TACHO_TPULE_RC_C = 0.00000001; // 10nF
constexpr float TACH_TPULSE_DT   = 0.6 * TACHO_TPULE_RC_R * TACHO_TPULE_RC_C; //from datasheet

#define TACHO_DRIVER_LEFT ICUD1
#define TACHO_DRIVER_RIGHT ICUD4

ICUConfig const leftTachoConfig {
    .mode = ICU_INPUT_ACTIVE_HIGH,
    .frequency = ICU_FREQUENCY,
    .width_cb = nullptr,
    .period_cb = nullptr,
    .overflow_cb = nullptr,
    .channel = ICU_CHANNEL_1,
    .dier = 0,
    .arr = 0,
};

ICUConfig const rightTachoConfig {
    .mode = ICU_INPUT_ACTIVE_HIGH,
    .frequency = ICU_FREQUENCY,
    .width_cb = nullptr,
    .period_cb = nullptr,
    .overflow_cb = nullptr,
    .channel = ICU_CHANNEL_1,
    .dier = 0,
    .arr = 0,
};

#define PWM_COUNTING_FREQUENCY 32000000
#define PWM_OUTPUT_FREQUENCY 20000

__extension__ const PWMChannelConfig channelConf{
    .mode     = PWM_OUTPUT_ACTIVE_HIGH,
    .callback = NULL,
};

__extension__ const PWMConfig pwmMotorConfig{
    .frequency = PWM_COUNTING_FREQUENCY,
    .period    = PWM_COUNTING_FREQUENCY / PWM_OUTPUT_FREQUENCY,
    .callback  = NULL,
    .channels  = {
        channelConf,
        channelConf,
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
    },
    .cr2  = 0,
    .bdtr = 0,
    .dier = 0,
};


#define ENCODER_LEFT_CHAN1_LINE PAL_LINE(GPIOA, 15U)
#define ENCODER_LEFT_CHAN2_LINE PAL_LINE(GPIOB, 8U)
#define ENCODER_RIGHT_CHAN1_LINE PAL_LINE(GPIOA, 0U)
#define ENCODER_RIGHT_CHAN2_LINE PAL_LINE(GPIOA, 1U)
#define TACHO_LEFT_LINE  PAL_LINE(GPIOB, 6U)
#define TACHO_RIGHT_LINE PAL_LINE(GPIOA, 8U)

#define ENCODER_LEFT_CHAN1_PIN_MODE  PAL_MODE_ALTERNATE(2)
#define ENCODER_LEFT_CHAN2_PIN_MODE  PAL_MODE_ALTERNATE(10)
#define ENCODER_RIGHT_CHAN1_PIN_MODE PAL_MODE_ALTERNATE(1)
#define ENCODER_RIGHT_CHAN2_PIN_MODE PAL_MODE_ALTERNATE(1)
#define TACHO_LEFT_PIN_MODE          PAL_MODE_ALTERNATE(5)
#define TACHO_RIGHT_PIN_MODE         PAL_MODE_ALTERNATE(6)

// Timers

#define MOTOR_CONTROL_LOOP_TIMER GPTD7

// GPIO
//#define LED_LINE PAL_LINE(GPIOB, 8U)
//#define LED_LINE_MODE PAL_MODE_OUTPUT_PUSHPULL

#define EMGCY_STOP_PIN PAL_LINE(GPIOA, 4U)
#define EMGCY_STOP_PIN_MODE PAL_MODE_INPUT_PULLUP

#define BRAKE_LEFT_PIN PAL_LINE(GPIOA, 6U)
#define BRAKE_LEFT_PIN_MODE PAL_MODE_OUTPUT_PUSHPULL

#define BRAKE_RIGHT_PIN PAL_LINE(GPIOA, 10U)
#define BRAKE_RIGHT_PIN_MODE PAL_MODE_OUTPUT_PUSHPULL
