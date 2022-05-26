#pragma once

#include <inttypes.h>
#include "LocalMath.hpp"
#include "RobotConf.hpp"

constexpr uint16_t LED_TOGGLE_FREQUENCY = 10; //[Hz]

constexpr uint16_t MOTOR_CONTROL_LOOP_FREQ = 100; //[Hz]
constexpr float    MOTOR_CONTROL_LOOP_DT   = 1./MOTOR_CONTROL_LOOP_FREQ; //[s]

constexpr float ENCODER_TICK_PER_TURN = 4096.;

constexpr float DEFAULT_MAX_PID_OUTPUT      = 1.;
constexpr float DEFAULT_MAX_PID_INTEGRAL    = 1.;

constexpr uint16_t PWM_DUTY_CYCLE_PRECISION = 100000;

constexpr float GEAR_RATIO     = 1.;
constexpr float GEAR_RATIO_INV = 1.;

constexpr float MAX_WHEEL_SPEED    = 400.; //[mm/s] //TODO: Change me!
constexpr float MAX_ANGULAR_SPEED  = 3/2*M_PI; //TODO: Change me!
constexpr float MAX_LINEAR_ACCL    = 400.; //[mm/s^2]
constexpr float MAX_ANGULAR_ACCL    = 2*M_PI; //[mm/s^2]
constexpr float MAX_WHEEL_ACCL     = 10000.; // [mm/s]œ

constexpr float COMPUTE_DIRECTION_THRESHOLD = 10.; //[mm] //TODO: Change me!

constexpr float ANGLE_PRECISION = 0.05; //[rad]
constexpr float DISTANCE_PRECISION = 5;  //[mm]

constexpr float CONTROL_COMMAND_TIMEOUT_S = 0.25; //[s]
