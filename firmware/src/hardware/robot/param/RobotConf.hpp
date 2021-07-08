#pragma once
#include "ch.h"
#include "hal.h"
#include <inttypes.h>

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

constexpr float INITIAL_X_POS       = 500.; //TODO: Change me!
constexpr float INITIAL_Y_POS       = 500.; //TODO: Change me!
constexpr float INITIAL_ANGLE       = 0.; //TODO: Change me!

constexpr float DISTANCE_KP         = 1.5; //TODO: Change me!
constexpr float DISTANCE_KI         = 0.05; //TODO: Change me!
constexpr float ANGLE_KP            = 2; //TODO: Change me!

#define MOTOR_LEFT_FORWARD   PAL_LOW
#define MOTOR_LEFT_BACKWARD  PAL_HIGH
#define MOTOR_RIGHT_FORWARD  PAL_HIGH
#define MOTOR_RIGHT_BACKWARD PAL_LOW


#elif defined(BRUT_ROBOT)

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

constexpr float INITIAL_X_POS       = 800.; //TODO: Change me!
constexpr float INITIAL_Y_POS       = 200.; //TODO: Change me!
constexpr float INITIAL_ANGLE       = M_PI/2; //TODO: Change me!

constexpr float DISTANCE_KP         = 2.; //TODO: Change me!
constexpr float DISTANCE_KI         = 0.05; //TODO: Change me!
constexpr float ANGLE_KP            = 5; //TODO: Change me!

#define MOTOR_LEFT_FORWARD   PAL_LOW
#define MOTOR_LEFT_BACKWARD  PAL_HIGH
#define MOTOR_RIGHT_FORWARD  PAL_HIGH
#define MOTOR_RIGHT_BACKWARD PAL_LOW

#else
#error "Define a robot!"
#endif /* define(YELLOW_ROBOT */
