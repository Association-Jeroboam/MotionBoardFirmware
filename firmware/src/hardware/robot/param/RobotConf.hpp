#pragma once

#include <inttypes.h>

#ifdef WHITE_ROBOT

constexpr float WHEEL_LEFT_RADIUS  = 32.;  //[mm] Theorical 32 //TODO: Change me!
constexpr float WHEEL_RIGHT_RADIUS = 32.;  //[mm] Theorical 32 //TODO: Change me!
constexpr float WHEEL_BASE         = 186.; //[mm] Theorical 186 //TODO: Change me!

constexpr float LEFT_MOTOR_KP      = 0.003; //TODO: Change me!
constexpr float LEFT_MOTOR_KI      = 0.03; //TODO: Change me!
constexpr float LEFT_MOTOR_KD      = 0.0; //TODO: Change me!
constexpr float LEFT_MOTOR_BIAS    = 0.1; //TODO: Change me!

constexpr float RIGHT_MOTOR_KP      = 0.003; //TODO: Change me!
constexpr float RIGHT_MOTOR_KI      = 0.03; //TODO: Change me!
constexpr float RIGHT_MOTOR_KD      = 0.0; //TODO: Change me!
constexpr float RIGHT_MOTOR_BIAS    = 0.1; //TODO: Change me!

constexpr float INITIAL_X_POS       = 1000.; //TODO: Change me!
constexpr float INITIAL_Y_POS       = 250.; //TODO: Change me!
constexpr float INITIAL_ANGLE       = M_PI/2.; //TODO: Change me!

constexpr float DISTANCE_KP         = 2.; //TODO: Change me!
constexpr float ANGLE_KP            = 5.; //TODO: Change me!

#elif defined(BRUT_ROBOT)

constexpr float WHEEL_LEFT_RADIUS  = 32.;  //[mm] Theorical 32 //TODO: Change me!
constexpr float WHEEL_RIGHT_RADIUS = 32.;  //[mm] Theorical 32 //TODO: Change me!
constexpr float WHEEL_BASE         = 186.; //[mm] Theorical 186 //TODO: Change me!

constexpr float LEFT_MOTOR_KP      = 0.003; //TODO: Change me!
constexpr float LEFT_MOTOR_KI      = 0.03; //TODO: Change me!
constexpr float LEFT_MOTOR_KD      = 0.0; //TODO: Change me!
constexpr float LEFT_MOTOR_BIAS    = 0.1; //TODO: Change me!

constexpr float RIGHT_MOTOR_KP      = 0.003; //TODO: Change me!
constexpr float RIGHT_MOTOR_KI      = 0.03; //TODO: Change me!
constexpr float RIGHT_MOTOR_KD      = 0.0; //TODO: Change me!
constexpr float RIGHT_MOTOR_BIAS    = 0.1; //TODO: Change me!

constexpr float INITIAL_X_POS       = 1000.; //TODO: Change me!
constexpr float INITIAL_Y_POS       = 250.; //TODO: Change me!
constexpr float INITIAL_ANGLE       = M_PI/2.; //TODO: Change me!

constexpr float DISTANCE_KP         = 2.; //TODO: Change me!
constexpr float ANGLE_KP            = 5.; //TODO: Change me!

#else
#error "Define a robot!"
#endif /* define(YELLOW_ROBOT */
