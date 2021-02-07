#pragma once
#include "PID.h"
#include "Parameters.hpp"
#include "BuildConf.hpp"

class Motor {
public:
    Motor(enum Board::IO::encoder encoder,
          enum Board::IO::motor motor);
    void updateControl();

    void setSpeed(float speed);
    float getSpeed();
    void setPID(float p, float i, float d, float bias, float frequency);
    void setPID(float p, float i, float d);

private:
    float m_speedSetpoint; // [rad/s]
    float m_speed;         // [rad/s]
    PID   m_speedPID;
    const enum Board::IO::encoder m_encoder;
    const enum Board::IO::motor   m_motor;

    void updateSpeed();
};
