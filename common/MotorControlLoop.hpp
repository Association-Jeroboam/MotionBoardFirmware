#ifndef MOTIONBOARDFIRMWARE_MOTORCONTROLLOOP_HPP
#define MOTIONBOARDFIRMWARE_MOTORCONTROLLOOP_HPP

#include "ch.hpp"
#include "Motor.hpp"
#include "Board.hpp"

constexpr uint16_t MOTOR_CONTROL_LOOP_WA = 0x100;

class MotorControlLoop: public chibios_rt::BaseStaticThread<MOTOR_CONTROL_LOOP_WA>,
                        public chibios_rt::EventListener {
public:
    MotorControlLoop();

    void main() override;

    Motor m_leftMotor;
    Motor m_rightMotor;

    void motorSetSpeed(enum Board::IO::motor motor, float speed);
    void motorSetPID(enum Board::IO::motor motor, float p, float i, float d);

};


#endif //MOTIONBOARDFIRMWARE_MOTORCONTROLLOOP_HPP
