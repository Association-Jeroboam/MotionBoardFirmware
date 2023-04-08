#pragma once

#include "MotionBoard.hpp"
#include "board.h"

// Communications
#define LOGGING_DRIVER SD2
#define SHELL_DRIVER LOGGING_DRIVER

#define LOGGING_TX_PIN PAL_LINE(GPIOA, 2U)
#define LOGGING_TX_PIN_MODE PAL_MODE_ALTERNATE(7)
#define LOGGING_RX_PIN PAL_LINE(GPIOA, 15U)
#define LOGGING_RX_PIN_MODE PAL_MODE_ALTERNATE(7)

__extension__ const SerialConfig lidarSDConfig{
    .speed = 115200,
    .cr1   = 0,
    .cr2   = USART_CR2_STOP1_BITS,
    .cr3   = 0};

#define MOTOR_PWM_DRIVER PWMD1

#define CAN_DRIVER CAND1
#define CAN_TX_PIN LINE_ARD_D2
#define CAN_TX_PIN_MODE PAL_MODE_ALTERNATE(9)
#define CAN_RX_PIN LINE_ARD_D10
#define CAN_RX_PIN_MODE PAL_MODE_ALTERNATE(9)

CANConfig const canConfig = {
    .mcr = 0,
    .btr = 0x001c0003,
};

#define MOTOR_LEFT_P_CHAN_LINE PAL_LINE(GPIOA, 9U)
#define MOTOR_LEFT_N_CHAN_LINE PAL_LINE(GPIOB, 0U)
#define MOTOR_RIGHT_P_CHAN_LINE PAL_LINE(GPIOA, 8U)
#define MOTOR_RIGHT_N_CHAN_LINE PAL_LINE(GPIOA, 7U)

#define MOTOR_LEFT_P_CHAN_LINE_MODE PAL_MODE_ALTERNATE(6)
#define MOTOR_LEFT_N_CHAN_LINE_MODE PAL_MODE_ALTERNATE(6)
#define MOTOR_RIGHT_P_CHAN_LINE_MODE PAL_MODE_ALTERNATE(6)
#define MOTOR_RIGHT_N_CHAN_LINE_MODE PAL_MODE_ALTERNATE(6)

constexpr uint8_t MOTOR_LEFT_CHANNEL  = 1;
constexpr uint8_t MOTOR_RIGHT_CHANNEL = 0;

#define PWM_COUNTING_FREQUENCY 20000000
#define PWM_OUTPUT_FREQUENCY 20000
#define DEADTIME_VALUE 40

__extension__ const PWMChannelConfig channelConf{
    .mode     = PWM_OUTPUT_ACTIVE_LOW | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW,
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
    .bdtr = DEADTIME_VALUE,
    .dier = 0,
};

#define LEFT_ENCODER_DRIVER QEID2
#define RIGHT_ENCODER_DRIVER QEID3

#define ENCODER_LEFT_CHAN1_LINE PAL_LINE(GPIOA, 0U)
#define ENCODER_LEFT_CHAN2_LINE PAL_LINE(GPIOA, 1U)
#define ENCODER_RIGHT_CHAN1_LINE PAL_LINE(GPIOA, 4U)
#define ENCODER_RIGHT_CHAN2_LINE PAL_LINE(GPIOA, 6U)

#define ENCODER_LEFT_CHAN1_PIN_MODE PAL_MODE_ALTERNATE(1)
#define ENCODER_LEFT_CHAN2_PIN_MODE PAL_MODE_ALTERNATE(1)
#define ENCODER_RIGHT_CHAN1_PIN_MODE PAL_MODE_ALTERNATE(2)
#define ENCODER_RIGHT_CHAN2_PIN_MODE PAL_MODE_ALTERNATE(2)

// Timers

#define MOTOR_CONTROL_LOOP_TIMER GPTD7
#define START_MATCH_TIMER GPTD15

// GPIO

#define LED_LINE LINE_ARD_D13
