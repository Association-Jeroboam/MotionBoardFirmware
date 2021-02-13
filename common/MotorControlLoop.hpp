#pragma once

#include "ch.hpp"
#include "Motor.hpp"
#include "Board.hpp"

constexpr uint16_t MOTOR_CONTROL_LOOP_WA = 0x100;

class MotorControlLoop : public chibios_rt::BaseStaticThread<MOTOR_CONTROL_LOOP_WA>,
                         public chibios_rt::EventListener {
public:

    static MotorControlLoop * instance();

    void main() override;

    void motorSetSpeed(enum Board::IO::motor motor, float speed);

    void motorSetPID(enum Board::IO::motor motor, float p, float i, float d);

private:
    explicit MotorControlLoop();

    Motor m_leftMotor;
    Motor m_rightMotor;

};
