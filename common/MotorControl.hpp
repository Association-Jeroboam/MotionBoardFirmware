#pragma once

#include "Motor.hpp"
#include "Peripherals.hpp"

class MotorControl {
  public:
    MotorControl();

    void update();

    void updateMeasure();

    void motorSetSpeed(Peripherals::Motor motor, float speed);

    void motorSetPID(Peripherals::Motor motor, float p, float i, uint8_t range);
    void motorSetPIDThreshold(Peripherals::Motor motor, float threshold, uint8_t range);

    float getMotorDrivenDistance(Peripherals::Motor motor);

    int32_t getMotorEncoderTicks(Peripherals::Motor motor);

    float getMotorSpeed(Peripherals::Motor motor);

    void setDisable(bool disable);

    void resetMotor(Peripherals::Motor motor);

    void setWheelRadius(Peripherals::Motor motor, float wheelRadius);

    SpeedControllerParameters getMotorControllerParameters(Peripherals::Motor motor);

  private:
    Motor m_leftMotor;
    Motor m_rightMotor;
};
