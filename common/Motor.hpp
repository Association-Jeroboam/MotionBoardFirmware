#pragma once

#include "PID.h"
#include "AdaptativeSpeedController.hpp"
#include "Peripherals.hpp"

class Motor {
  public:
    Motor(Peripherals::Encoder encoder,
          Peripherals::Motor   motor,
          float                wheelRadius);

    void updateControl();

    void setSpeed(float speed);

//    void setPID(float p, float i, float d, float bias, float frequency);

//    void setPID(float p, float i, float d);

    void setPID(float p, float i, uint8_t range);

    void setWheelRadius(float wheelRadius);

    void setDisable(bool disable);

    float getSpeed();

    float getDrivenDistance();

    void reset();

  private:
    float                      m_speedSetpoint; // [mm/s]
    float                      m_speed;         // [mm/s]
    float                      m_drivenDistance;
    float                      m_wheelRadius;
    PID                        m_speedPID;
    AdaptativeSpeedController  m_speedController;
    const Peripherals::Encoder m_encoder;
    const Peripherals::Motor   m_motor;
    bool m_disabled;

    void updateMeasure();
};
