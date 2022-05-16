#pragma once

#include "Motor.hpp"
#include "Peripherals.hpp"

class MotorControl {
  public:
    MotorControl();

    void update();

    void motorSetSpeed(Peripherals::Motor motor, float speed);

    void motorSetPID(Peripherals::Motor motor, float p, float i, float d);

    float getMotorDrivenDistance(Peripherals::Motor motor);

    float getMotorSpeed(Peripherals::Motor motor);

    void setDisable(bool disable);

    void resetMotor(Peripherals::Motor motor);

    void setWheelRadius(Peripherals::Motor motor, float wheelRadius);

  private:
    Motor m_leftMotor;
    Motor m_rightMotor;
};
