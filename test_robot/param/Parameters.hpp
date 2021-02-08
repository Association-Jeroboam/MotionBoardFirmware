#pragma once

#include <inttypes.h>

constexpr uint16_t MOTOR_CONTROL_LOOP_FREQ = 100; //[Hz]
constexpr float    MOTOR_CONTROL_LOOP_DT   = 1/MOTOR_CONTROL_LOOP_FREQ; //[s]

constexpr float ENCODER_TICK_PER_TURN = 48.;

constexpr float GEAR_RATIO_NUM = 120000.;  // 22*22*22*22*24
constexpr float GEAR_RATIO_DEN = 5622144.; // 12*10*10*10*10
constexpr float GEAR_RATIO     = ((GEAR_RATIO_NUM) / (GEAR_RATIO_DEN));
constexpr float GEAR_RATIO_INV = ((GEAR_RATIO_DEN) / (GEAR_RATIO_NUM));

constexpr float WHEEL_LEFT_RADIUS  = 0.064; //[m] Theorical 0.064
constexpr float WHEEL_RIGHT_RADIUS = 0.064; //[m] Theorical 0.064
constexpr float WHEEL_BASE         = 0.186; //[m] Theorical 0.186
