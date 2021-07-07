#pragma once

#include "Motor.hpp"
#include "Peripherals.hpp"

class MotorControl {
  public:
    MotorControl();

    void updateControl();

    void updateMeasure();

    void motorSetSpeed(Peripherals::Motor motor, float speed);

    float getMotorDrivenDistance(Peripherals::Motor motor);

    float getMotorSpeed(Peripherals::Motor motor);

    void resetMotor(Peripherals::Motor motor);

  private:
    Motor m_leftMotor;
    Motor m_rightMotor;
};
