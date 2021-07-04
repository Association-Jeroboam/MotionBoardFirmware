#pragma once

#include "Board.hpp"
#define LOGGING_DRIVER SD2
#define SHELL_DRIVER LOGGING_DRIVER
#define MOTOR_PWM_DRIVER PWMD1  //right for now
#define MOTOR_PWM_DRIVER2 PWMD4 //left for now

#define LOGGING_TX_PIN PAL_LINE(GPIOA, 2U)
#define LOGGING_TX_PIN_MODE PAL_MODE_ALTERNATE(7)
#define LOGGING_RX_PIN PAL_LINE(GPIOA, 3U)
#define LOGGING_RX_PIN_MODE PAL_MODE_ALTERNATE(7)

#define LIDAR_SD_DRIVER SD1

#define LIDAR_SD_TX_PIN PAL_LINE(GPIOB, 6U)
#define LIDAR_SD_TX_PIN_MODE PAL_MODE_ALTERNATE(7)
#define LIDAR_SD_RX_PIN PAL_LINE(GPIOB, 7U)
#define LIDAR_SD_RX_PIN_MODE PAL_MODE_ALTERNATE(7)

__extension__ const SerialConfig lidarSDConfig {
        .speed = 115200,
        .cr1 = 0,
        .cr2 = USART_CR2_STOP1_BITS,
        .cr3 = 0
};

#define CAN_DRIVER CAND1
#define CAN_TX_PIN      PAL_LINE(GPIOA, 12U)
#define CAN_TX_PIN_MODE PAL_MODE_ALTERNATE(9)
#define CAN_RX_PIN      PAL_LINE(GPIOA, 11U)
#define CAN_RX_PIN_MODE PAL_MODE_ALTERNATE(9)

CANConfig const canConfig = {
    .NBTP = 0x2070C01,
    .DBTP = 0,
    .CCCR = 0,
    .TEST = 0,
    .RXGFC = 0
};

#define CAN_CLK_PRESCALER 16 << FDCAN_DBTP_DBRP_Pos
#define CAN_DTSEG1        12 << FDCAN_DBTP_DTSEG1_Pos
#define CAN_DTSEG2        1 << FDCAN_DBTP_DTSEG2_Pos
#define CAN_DSJW          0


#define MOTOR_LEFT_P_CHAN_LINE  PAL_LINE(GPIOA, 10U)
#define MOTOR_RIGHT_P_CHAN_LINE PAL_LINE(GPIOA, 8U)

#define MOTOR_LEFT_P_CHAN_LINE_MODE  PAL_MODE_ALTERNATE(6)
#define MOTOR_RIGHT_P_CHAN_LINE_MODE PAL_MODE_ALTERNATE(6)

#define MOTOR_LEFT_DIR_PIN  PAL_LINE(GPIOA, 15U)
#define MOTOR_LEFT_DIR_PIN_MODE  PAL_MODE_OUTPUT_PUSHPULL
#define MOTOR_RIGHT_DIR_PIN PAL_LINE(GPIOB, 3U)
#define MOTOR_RIGHT_DIR_PIN_MODE PAL_MODE_OUTPUT_PUSHPULL

#define PWM_COUNTING_FREQUENCY 1000000
#define PWM_OUTPUT_FREQUENCY   10000

__extension__ const PWMChannelConfig channelConf{
    .mode     = PWM_OUTPUT_ACTIVE_HIGH,
    .callback = NULL,
};

constexpr uint8_t MOTOR_LEFT_CHANNEL  = 2;
constexpr uint8_t MOTOR_RIGHT_CHANNEL = 0;

__extension__ const PWMConfig pwmMotorConfig{
    .frequency = PWM_COUNTING_FREQUENCY,
    .period    = PWM_COUNTING_FREQUENCY / PWM_OUTPUT_FREQUENCY,
    .callback  = NULL,
    .channels  = {
        channelConf,
        {PWM_OUTPUT_DISABLED, NULL},
        channelConf,
        {PWM_OUTPUT_DISABLED, NULL},
    },
    .cr2  = 0,
    .bdtr = 0,
    .dier = 0,
};

#define LEFT_ENCODER_DRIVER QEID3
#define RIGHT_ENCODER_DRIVER QEID2

#define ENCODER_LEFT_CHAN1_LINE PAL_LINE(GPIOB, 4U)
#define ENCODER_LEFT_CHAN2_LINE PAL_LINE(GPIOA, 7U)
#define ENCODER_RIGHT_CHAN1_LINE PAL_LINE(GPIOA, 0U)
#define ENCODER_RIGHT_CHAN2_LINE PAL_LINE(GPIOA, 1U)

#define ENCODER_LEFT_CHAN1_PIN_MODE PAL_MODE_ALTERNATE(2)
#define ENCODER_LEFT_CHAN2_PIN_MODE PAL_MODE_ALTERNATE(2)
#define ENCODER_RIGHT_CHAN1_PIN_MODE PAL_MODE_ALTERNATE(1)
#define ENCODER_RIGHT_CHAN2_PIN_MODE PAL_MODE_ALTERNATE(1)

// Timers

#define MOTOR_CONTROL_LOOP_TIMER GPTD7
//#define START_MATCH_TIMER GPTD15

// GPIO
#define LED_LINE            PAL_LINE(GPIOB, 8U)
#define LED_LINE_MODE       PAL_MODE_OUTPUT_PUSHPULL

#define START_PIN           PAL_LINE(GPIOB, 0U)
#define START_PIN_MODE      PAL_MODE_INPUT_PULLUP

#define STRATEGY_1_PIN      PAL_LINE(GPIOA, 6U)
#define STRATEGY_1_PIN_MODE PAL_MODE_INPUT_PULLUP

#define STRATEGY_2_PIN      PAL_LINE(GPIOA, 5U)
#define STRATEGY_2_PIN_MODE PAL_MODE_INPUT_PULLUP

#define SIDE_PIN            PAL_LINE(GPIOA, 4U)
#define SIDE_PIN_MODE       PAL_MODE_INPUT_PULLUP

#define EMGCY_STOP_PIN            PAL_LINE(GPIOB, 5U)
#define EMGCY_STOP_PIN_MODE       PAL_MODE_INPUT_PULLUP
