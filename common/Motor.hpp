#pragma once

#include "PID.h"

#include "Peripherals.hpp"

class Motor {
  public:
    Motor(Peripherals::Encoder encoder,
          Peripherals::Motor   motor,
          float                wheelRadius);

    void updateControl();

    void updateMeasure();

    void setSpeed(float speed);

    void setWheelRadius(float wheelRadius);

    float getSpeed();

    float getDrivenDistance();

    void reset();

  private:
    float                      m_speedSetpoint; // [pwm]
    float                      m_speed;         // [mm/s]
    float                      m_drivenDistance;
    float                      m_wheelRadius;
    const Peripherals::Encoder m_encoder;
    const Peripherals::Motor   m_motor;
};
