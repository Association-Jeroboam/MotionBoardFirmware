#pragma once

#include <inttypes.h>
#include "LocalMath.hpp"

constexpr uint16_t LED_TOGGLE_FREQUENCY = 10; //[Hz]

constexpr uint16_t MOTOR_CONTROL_LOOP_FREQ = 100; //[Hz]
constexpr float    MOTOR_CONTROL_LOOP_DT   = 1./MOTOR_CONTROL_LOOP_FREQ; //[s]

constexpr float ENCODER_TICK_PER_TURN = 48.; //TODO: Change me!

constexpr float DEFAULT_MAX_PID_OUTPUT      = 1.;
constexpr float DEFAULT_MAX_PID_INTEGRAL    = 1.;

constexpr uint16_t PWM_MAX_DUTY_CYCLE_VALUE = 10000;

constexpr float GEAR_RATIO_NUM = 120000.;  // 22*22*22*22*24 //TODO: Change me!
constexpr float GEAR_RATIO_DEN = 5622144.; // 12*10*10*10*10 //TODO: Change me!
constexpr float GEAR_RATIO     = ((GEAR_RATIO_NUM) / (GEAR_RATIO_DEN));
constexpr float GEAR_RATIO_INV = ((GEAR_RATIO_DEN) / (GEAR_RATIO_NUM));

constexpr float WHEEL_LEFT_RADIUS  = 32.;  //[mm] Theorical 32 //TODO: Change me!
constexpr float WHEEL_RIGHT_RADIUS = 32.;  //[mm] Theorical 32 //TODO: Change me!
constexpr float WHEEL_BASE         = 186.; //[mm] Theorical 186 //TODO: Change me!

constexpr float MAX_WHEEL_SPEED    = 550.; //[mm/s] //TODO: Change me!
constexpr float MAX_ANGULAR_SPEED  = M_PI; //TODO: Change me!

constexpr float LEFT_MOTOR_KP      = 0.003; //TODO: Change me!
constexpr float LEFT_MOTOR_KI      = 0.03; //TODO: Change me!
constexpr float LEFT_MOTOR_KD      = 0.0; //TODO: Change me!
constexpr float LEFT_MOTOR_BIAS    = 0.1; //TODO: Change me!

constexpr float RIGHT_MOTOR_KP      = 0.003; //TODO: Change me!
constexpr float RIGHT_MOTOR_KI      = 0.03; //TODO: Change me!
constexpr float RIGHT_MOTOR_KD      = 0.0; //TODO: Change me!
constexpr float RIGHT_MOTOR_BIAS    = 0.1; //TODO: Change me!

constexpr float INITIAL_X_POS       = 0.; //TODO: Change me!
constexpr float INITIAL_Y_POS       = 0.; //TODO: Change me!
constexpr float INITIAL_ANGLE       = 0.; //TODO: Change me!

constexpr float DISTANCE_KP         = 2.; //TODO: Change me!
constexpr float ANGLE_KP            = 5.; //TODO: Change me!

constexpr float COMPUTE_DIRECTION_THRESHOLD = 10.; //[mm] //TODO: Change me!

constexpr float ANGLE_PRECISION = 0.01; //[rad]
constexpr float DISTANCE_PRECISION = 5;  //[mm]
