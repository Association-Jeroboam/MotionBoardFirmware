#pragma once
#include "ch.h"
#include "hal.h"
#include <inttypes.h>
#include <climits>

#ifdef WHITE_ROBOT

constexpr float WHEEL_RADIUS_FACTOR = 1.008926272;
constexpr float WHEEL_DISTANCE_FACTOR = 0.9879532858233885;

constexpr float WHEEL_LEFT_RADIUS  = 28.25 * WHEEL_DISTANCE_FACTOR;  //[mm] Theorical 28.25
constexpr float WHEEL_RIGHT_RADIUS = 28.25 / WHEEL_RADIUS_FACTOR * WHEEL_DISTANCE_FACTOR;  //[mm] Theorical 28.25
constexpr float WHEEL_BASE         = 266.; //[mm] Theorical 266

constexpr float LEFT_MOTOR_KP      = 0.005; //TODO: Change me!
constexpr float LEFT_MOTOR_KI      = 0.001; //TODO: Change me!
constexpr float LEFT_MOTOR_KD      = 0.0; //TODO: Change me!
constexpr float LEFT_MOTOR_BIAS    = 0.15; //TODO: Change me!

constexpr float RIGHT_MOTOR_KP      = 0.005; //TODO: Change me!
constexpr float RIGHT_MOTOR_KI      = 0.001; //TODO: Change me!
constexpr float RIGHT_MOTOR_KD      = 0.0; //TODO: Change me!
constexpr float RIGHT_MOTOR_BIAS    = 0.15; //TODO: Change me!

constexpr float INITIAL_X_POS       = 655.; //TODO: Change me!
constexpr float INITIAL_Y_POS       = 230.; //TODO: Change me!
constexpr float INITIAL_ANGLE       = M_PI.; //TODO: Change me!

constexpr float KP         = 0; //TODO: Change me!
constexpr float KA         = 0; //TODO: Change me!
constexpr float KB         = -0.6; //TODO: Change me!

#define MOTOR_LEFT_FORWARD   PAL_HIGH
#define MOTOR_LEFT_BACKWARD  PAL_LOW
#define MOTOR_RIGHT_FORWARD  PAL_LOW
#define MOTOR_RIGHT_BACKWARD PAL_HIGH

#define LEFT_ENCODER_DRIVER QEID3
#define RIGHT_ENCODER_DRIVER QEID2

__extension__ const QEIConfig leftEncoderConf{
    .mode        = QEI_MODE_QUADRATURE,
    .resolution  = QEI_BOTH_EDGES,
    .dirinv      = QEI_DIRINV_TRUE,
    .overflow    = QEI_OVERFLOW_WRAP,
    .min         = 0,
    .max         = SHRT_MAX,
    .notify_cb   = NULL,
    .overflow_cb = NULL,
};

__extension__ const QEIConfig rightEncoderConf{
    .mode        = QEI_MODE_QUADRATURE,
    .resolution  = QEI_BOTH_EDGES,
    .dirinv      = QEI_DIRINV_FALSE,
    .overflow    = QEI_OVERFLOW_WRAP,
    .min         = 0,
    .max         = SHRT_MAX,
    .notify_cb   = NULL,
    .overflow_cb = NULL,
};

constexpr uint8_t MOTOR_LEFT_CHANNEL  = 2;
constexpr uint8_t MOTOR_RIGHT_CHANNEL = 0;

#define MOTOR_LEFT_P_CHAN_LINE PAL_LINE(GPIOA, 10U)
#define MOTOR_RIGHT_P_CHAN_LINE PAL_LINE(GPIOA, 8U)

#define MOTOR_LEFT_P_CHAN_LINE_MODE PAL_MODE_ALTERNATE(6)
#define MOTOR_RIGHT_P_CHAN_LINE_MODE PAL_MODE_ALTERNATE(6)

#define MOTOR_LEFT_DIR_PIN PAL_LINE(GPIOB, 3U)
#define MOTOR_LEFT_DIR_PIN_MODE PAL_MODE_OUTPUT_PUSHPULL
#define MOTOR_RIGHT_DIR_PIN PAL_LINE(GPIOA, 15U)
#define MOTOR_RIGHT_DIR_PIN_MODE PAL_MODE_OUTPUT_PUSHPULL


#elif defined(BRUT_ROBOT)

constexpr float WHEEL_RADIUS_FACTOR = 1.008926272;
constexpr float WHEEL_DISTANCE_FACTOR = 0.9879532858233885;

constexpr float WHEEL_LEFT_RADIUS  = 28.25 * WHEEL_DISTANCE_FACTOR;  //[mm] Theorical 28.25
constexpr float WHEEL_RIGHT_RADIUS = 28.25 / WHEEL_RADIUS_FACTOR * WHEEL_DISTANCE_FACTOR;  //[mm] Theorical 28.25
constexpr float WHEEL_BASE         = 266.; //[mm] Theorical 266

constexpr float LEFT_MOTOR_KP      = 0.004; //TODO: Change me!
constexpr float LEFT_MOTOR_KI      = 0.002; //TODO: Change me!
constexpr float LEFT_MOTOR_KD      = 0.0; //TODO: Change me!
constexpr float LEFT_MOTOR_BIAS    = 0.15; //TODO: Change me!

constexpr float RIGHT_MOTOR_KP      = 0.003; //TODO: Change me!
constexpr float RIGHT_MOTOR_KI      = 0.0022; //TODO: Change me!
constexpr float RIGHT_MOTOR_KD      = 0.0; //TODO: Change me!
constexpr float RIGHT_MOTOR_BIAS    = 0.15; //TODO: Change me!

constexpr float INITIAL_X_POS       = 655.; //TODO: Change me!
constexpr float INITIAL_Y_POS       = 230.; //TODO: Change me!
constexpr float INITIAL_ANGLE       = 0; //TODO: Change me!

constexpr float KP         = 0; //TODO: Change me!
constexpr float KA         = 0; //TODO: Change me!
constexpr float KB         = -0.6; //TODO: Change me!

#define MOTOR_LEFT_FORWARD   PAL_LOW
#define MOTOR_LEFT_BACKWARD  PAL_HIGH
#define MOTOR_RIGHT_FORWARD  PAL_HIGH
#define MOTOR_RIGHT_BACKWARD PAL_LOW

#define LEFT_ENCODER_DRIVER QEID3
#define RIGHT_ENCODER_DRIVER QEID2

__extension__ const QEIConfig leftEncoderConf{
    .mode        = QEI_MODE_QUADRATURE,
    .resolution  = QEI_BOTH_EDGES,
    .dirinv      = QEI_DIRINV_TRUE,
    .overflow    = QEI_OVERFLOW_WRAP,
    .min         = 0,
    .max         = SHRT_MAX,
    .notify_cb   = NULL,
    .overflow_cb = NULL,
};

__extension__ const QEIConfig rightEncoderConf{
    .mode        = QEI_MODE_QUADRATURE,
    .resolution  = QEI_BOTH_EDGES,
    .dirinv      = QEI_DIRINV_FALSE,
    .overflow    = QEI_OVERFLOW_WRAP,
    .min         = 0,
    .max         = SHRT_MAX,
    .notify_cb   = NULL,
    .overflow_cb = NULL,
};

constexpr uint8_t MOTOR_LEFT_CHANNEL  = 2;
constexpr uint8_t MOTOR_RIGHT_CHANNEL = 0;

#define MOTOR_LEFT_P_CHAN_LINE PAL_LINE(GPIOA, 10U)
#define MOTOR_RIGHT_P_CHAN_LINE PAL_LINE(GPIOA, 8U)

#define MOTOR_LEFT_P_CHAN_LINE_MODE PAL_MODE_ALTERNATE(6)
#define MOTOR_RIGHT_P_CHAN_LINE_MODE PAL_MODE_ALTERNATE(6)

#define MOTOR_LEFT_DIR_PIN PAL_LINE(GPIOB, 3U)
#define MOTOR_LEFT_DIR_PIN_MODE PAL_MODE_OUTPUT_PUSHPULL
#define MOTOR_RIGHT_DIR_PIN PAL_LINE(GPIOA, 15U)
#define MOTOR_RIGHT_DIR_PIN_MODE PAL_MODE_OUTPUT_PUSHPULL

#else
#error "Define a robot!"
#endif /* define(YELLOW_ROBOT */
