#pragma once

#include <inttypes.h>
#include "LocalMath.hpp"

constexpr uint16_t LED_TOGGLE_FREQUENCY = 10; //[Hz]

constexpr uint16_t MOTOR_CONTROL_LOOP_FREQ = 100; //[Hz]
constexpr float    MOTOR_CONTROL_LOOP_DT   = 1./MOTOR_CONTROL_LOOP_FREQ; //[s]

constexpr float ENCODER_TICK_PER_TURN = 48.;

constexpr float DEFAULT_MAX_PID_OUTPUT      = 1.;
constexpr float DEFAULT_MAX_PID_INTEGRAL    = 1.;

constexpr uint16_t PWM_MAX_DUTY_CYCLE_VALUE = 10000;

constexpr float GEAR_RATIO_NUM = 120000.;  // 22*22*22*22*24
constexpr float GEAR_RATIO_DEN = 5622144.; // 12*10*10*10*10
constexpr float GEAR_RATIO     = ((GEAR_RATIO_NUM) / (GEAR_RATIO_DEN));
constexpr float GEAR_RATIO_INV = ((GEAR_RATIO_DEN) / (GEAR_RATIO_NUM));

constexpr float WHEEL_LEFT_RADIUS  = 32.;  //[mm] Theorical 32
constexpr float WHEEL_RIGHT_RADIUS = 32.;  //[mm] Theorical 32
constexpr float WHEEL_BASE         = 186.; //[mm] Theorical 186

constexpr float MAX_WHEEL_SPEED    = 550.; //[mm/s]
constexpr float MAX_ANGULAR_SPEED  = M_PI;

constexpr float LEFT_MOTOR_KP      = 0.003;
constexpr float LEFT_MOTOR_KI      = 0.03;
constexpr float LEFT_MOTOR_KD      = 0.0;
constexpr float LEFT_MOTOR_BIAS    = 0.1;

constexpr float RIGHT_MOTOR_KP      = 0.003;
constexpr float RIGHT_MOTOR_KI      = 0.03;
constexpr float RIGHT_MOTOR_KD      = 0.0;
constexpr float RIGHT_MOTOR_BIAS    = 0.1;

constexpr float INITIAL_X_POS       = 0.;
constexpr float INITIAL_Y_POS       = 0.;
constexpr float INITIAL_ANGLE       = 0.;

constexpr float DISTANCE_KP         = 2.;
constexpr float ANGLE_KP            = 5.;

constexpr float COMPUTE_DIRECTION_THRESHOLD = 10.; //[mm]
