#pragma once

#include "ch.hpp"
#include "Motor.hpp"
#include "Board.hpp"

class MotorControl {
public:
    MotorControl();

    void update();

    void motorSetSpeed(enum Board::IO::motor motor, float speed);

    void motorSetPID(enum Board::IO::motor motor, float p, float i, float d);

    float getMotorDrivenDistance(enum Board::IO::motor motor);

    float getMotorSpeed(enum Board::IO::motor motor);

private:

    Motor m_leftMotor;
    Motor m_rightMotor;

};
