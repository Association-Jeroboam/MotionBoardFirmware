    #pragma once
#include "ch.h"
#include "hal.h"
#include <inttypes.h>
#include <climits>

constexpr float WHEEL_RADIUS_FACTOR = 1.0;
constexpr float WHEEL_DISTANCE_FACTOR = 1.0;

constexpr float WHEEL_LEFT_RADIUS  = 24. * WHEEL_DISTANCE_FACTOR;  //[mm] Theorical 28.25
constexpr float WHEEL_RIGHT_RADIUS = 24. * WHEEL_RADIUS_FACTOR * WHEEL_DISTANCE_FACTOR;  //[mm] Theorical 28.25
constexpr float WHEEL_BASE         = 222.; //[mm] Theorical 266

constexpr float LEFT_MOTOR_KP[3]      = {0.001,0.001,0.001};
constexpr float LEFT_MOTOR_KI[3]      = {0.0001,0.0001,0.0001};
constexpr float LEFT_MOTOR_KD      = 0.0;
constexpr float LEFT_MOTOR_BIAS    = 0.0;

constexpr float MOTOR_SPEED_RANGE[3]   = {10., 100., 500.};

constexpr float RIGHT_MOTOR_KP[3]      = {0.001,0.001,0.001};
constexpr float RIGHT_MOTOR_KI[3]      = {0.0001,0.0001,0.0001};
constexpr float RIGHT_MOTOR_KD      = 0.0;
constexpr float RIGHT_MOTOR_BIAS    = 0.0;

constexpr float LINEAR_CONTROL_KP[3]      = {0.001,0.001,0.001};
constexpr float LINEAR_CONTROL_KI[3]      = {0.0001,0.0001,0.0001};
constexpr float LINEAR_CONTROL_KD      = 0.0;
constexpr float LINEAR_CONTROL_BIAS    = 0.0;

constexpr float ANGULAR_CONTROL_KP[3]      = {0.001,0.001,0.001};
constexpr float ANGULAR_CONTROL_KI[3]      = {0.0001,0.0001,0.0001};
constexpr float ANGULAR_CONTROL_KD      = 0.0;
constexpr float ANGULAR_CONTROL_BIAS    = 0.0;

constexpr float INITIAL_X_POS       = 0.;
constexpr float INITIAL_Y_POS       = 0.;
constexpr float INITIAL_ANGLE       = 0;

constexpr float KP         = 2; //TODO: Change me!
constexpr float KA         = 4; //TODO: Change me!
constexpr float KB         = -4.5;// -4.5; //TODO: Change me!

#define MOTOR_LEFT_FORWARD   PAL_LOW
#define MOTOR_LEFT_BACKWARD  PAL_HIGH
#define MOTOR_RIGHT_FORWARD  PAL_HIGH
#define MOTOR_RIGHT_BACKWARD PAL_LOW

#define LEFT_ENCODER_DRIVER QEID8
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

constexpr uint8_t MOTOR_LEFT_CHANNEL  = 0;
constexpr uint8_t MOTOR_RIGHT_CHANNEL = 1;

#define MOTOR_LEFT_P_CHAN_LINE  PAL_LINE(GPIOC, 0U)
#define MOTOR_LEFT_N_CHAN_LINE  PAL_LINE(GPIOA, 11U)
#define MOTOR_RIGHT_P_CHAN_LINE PAL_LINE(GPIOC, 1U)
#define MOTOR_RIGHT_N_CHAN_LINE PAL_LINE(GPIOA, 12U)

#define MOTOR_LEFT_P_CHAN_LINE_MODE  PAL_MODE_ALTERNATE(2)
#define MOTOR_LEFT_N_CHAN_LINE_MODE  PAL_MODE_ALTERNATE(6)
#define MOTOR_RIGHT_P_CHAN_LINE_MODE PAL_MODE_ALTERNATE(2)
#define MOTOR_RIGHT_N_CHAN_LINE_MODE PAL_MODE_ALTERNATE(6)

#define MOTOR_LEFT_DIR_PIN PAL_LINE(GPIOB, 5U)
#define MOTOR_LEFT_DIR_PIN_MODE PAL_MODE_OUTPUT_PUSHPULL
#define MOTOR_RIGHT_DIR_PIN PAL_LINE(GPIOA, 5U)
#define MOTOR_RIGHT_DIR_PIN_MODE PAL_MODE_OUTPUT_PUSHPULL
