#pragma once

#include <inttypes.h>

constexpr uint16_t MOTOR_CONTROL_LOOP_FREQ = 100; //[Hz]
constexpr float    MOTOR_CONTROL_LOOP_DT   = 1/MOTOR_CONTROL_LOOP_FREQ; //[s]

constexpr float ENCODER_TICK_PER_TURN = 48.;

constexpr float MAX_PID_OUTPUT              = 1.;
constexpr uint16_t PWM_MAX_DUTY_CYCLE_VALUE = 10000;

constexpr float GEAR_RATIO_NUM = 120000.;  // 22*22*22*22*24
constexpr float GEAR_RATIO_DEN = 5622144.; // 12*10*10*10*10
constexpr float GEAR_RATIO     = ((GEAR_RATIO_NUM) / (GEAR_RATIO_DEN));
constexpr float GEAR_RATIO_INV = ((GEAR_RATIO_DEN) / (GEAR_RATIO_NUM));

constexpr float WHEEL_LEFT_RADIUS  = 0.064; //[m] Theorical 0.064
constexpr float WHEEL_RIGHT_RADIUS = 0.064; //[m] Theorical 0.064
constexpr float WHEEL_BASE         = 0.186; //[m] Theorical 0.186

constexpr float LEFT_MOTOR_KP      = 0.15;
constexpr float LEFT_MOTOR_KI      = 1.5;
constexpr float LEFT_MOTOR_KD      = 0.0;
constexpr float LEFT_MOTOR_BIAS    = 0.2;

constexpr float RIGHT_MOTOR_KP      = 0.15;
constexpr float RIGHT_MOTOR_KI      = 1.5;
constexpr float RIGHT_MOTOR_KD      = 0.0;
constexpr float RIGHT_MOTOR_BIAS    = 0.2;
