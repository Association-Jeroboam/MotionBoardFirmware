#pragma once

#include <inttypes.h>

constexpr uint16_t MOTOR_CONTROL_LOOP_FREQ = 100; //[Hz]
constexpr float    MOTOR_CONTROL_LOOP_DT   = 1/MOTOR_CONTROL_LOOP_FREQ; //[s]

constexpr float ENCODER_TICK_PER_TURN = 2048.;

constexpr float GEAR_RATIO     = 1.;
constexpr float GEAR_RATIO_INV = 1./GEAR_RATIO;

constexpr float WHEEL_LEFT_RADIUS  = 1.; //[m] change me!
constexpr float WHEEL_RIGHT_RADIUS = 1.; //[m] change me!
constexpr float WHEEL_BASE         = 1.; //[m] change me!
