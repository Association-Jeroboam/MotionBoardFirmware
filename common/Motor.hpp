#pragma once

#include "PID.h"
#include "AdaptativeSpeedController.hpp"
#include "Peripherals.hpp"
#include "Pll.h"

constexpr float DEFAULT_PLL_BW = 60; //[Hz]

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
    void setPIDThreshold(float threshold, uint8_t range);

    inline SpeedControllerParameters getControllerParameters(){return m_speedController.getParameters();};

    void setWheelRadius(float wheelRadius);

    void setDisable(bool disable);

    float getSpeed();

    float getDrivenDistance();

    int32_t getTickCount();

    void reset();

  private:
    float                      m_speed;         // [mm/s]
    float                      m_drivenDistance;
    float                      m_wheelRadius;
    PID                        m_speedPID;
    AdaptativeSpeedController  m_speedController;
    const Peripherals::Encoder m_encoder;
    const Peripherals::Motor   m_motor;
    bool m_disabled;
    int32_t                    m_tick_count;
    int32_t                    m_tick_integral;
    Pll                        m_pll;

    void updateMeasure();
};
