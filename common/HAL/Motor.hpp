#pragma once

#include "PID.h"

#include "BuildConf.hpp"

class Motor {
public:
    Motor(enum Board::IO::encoder encoder,
          enum Board::IO::motor motor,
          float wheelRadius);

    void updateControl();

    void setSpeed(float speed);

    void setPID(float p, float i, float d, float bias, float frequency);

    void setPID(float p, float i, float d);

    void setWheelRadius(float wheelRadius);

    float getSpeed();

    float getDrivenDistance();

private:
    float m_speedSetpoint; // [rad/s]
    float m_speed;         // [rad/s]
    float m_drivenDistance;
    float m_wheelRadius;
    PID m_speedPID;
    const enum Board::IO::encoder m_encoder;
    const enum Board::IO::motor m_motor;

    void updateSpeed();
};
