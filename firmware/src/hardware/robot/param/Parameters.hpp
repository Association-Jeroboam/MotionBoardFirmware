#pragma once

#include <inttypes.h>
#include "LocalMath.hpp"
#include "RobotConf.hpp"

constexpr uint16_t LED_TOGGLE_FREQUENCY = 10; //[Hz]

constexpr uint16_t MOTOR_CONTROL_LOOP_FREQ = 50; //[Hz]
constexpr float    MOTOR_CONTROL_LOOP_DT   = 1./MOTOR_CONTROL_LOOP_FREQ; //[s]

constexpr float ENCODER_TICK_PER_TURN = 4096.;

constexpr float DEFAULT_MAX_PID_OUTPUT      = 1.;
constexpr float DEFAULT_MAX_PID_INTEGRAL    = 1.;

constexpr uint16_t PWM_MAX_DUTY_CYCLE_VALUE = 10000;

constexpr float GEAR_RATIO     = 1.;
constexpr float GEAR_RATIO_INV = 1.;

constexpr float MAX_WHEEL_SPEED    = 800.; //[mm/s] //TODO: Change me!
constexpr float MAX_ANGULAR_SPEED  = 3/2*M_PI; //TODO: Change me!
constexpr float MAX_LINEAR_ACCL    = 800.; //[mm/s^2]
constexpr float MAX_ANGULAR_ACCL    = 2*M_PI; //[mm/s^2]
constexpr float MAX_WHEEL_ACCL     = 10000.; // [mm/s]œ

constexpr float COMPUTE_DIRECTION_THRESHOLD = 10.; //[mm] //TODO: Change me!

constexpr float ANGLE_PRECISION = 0.01; //[rad]
constexpr float DISTANCE_PRECISION = 2;  //[mm]

constexpr float LIDAR_ANGLE_OFFSET = -M_PI/2.; // [rad]
constexpr float LIDAR_X_OFFSET = 0.; // [mm]
constexpr float LIDAR_Y_OFFSET = 0.; // [mm]
constexpr float LIDAR_MIN_DISTANCE = 160.; // [mm]
constexpr float LIDAR_MAX_DISTANCE = 500.; // [mm]

constexpr float MAP_MAX_X = 2000.;
constexpr float MAP_MAX_Y = 3000.;

constexpr uint32_t COMPASS_TIMEOUT = 5;
constexpr uint32_t FLAG_TIMEOUT = 10;
constexpr uint32_t MATCH_TIMEOUT = 15;
