#pragma once

#include "ch.hpp"
#include "Goal.hpp"
#include "RobotPose.hpp"
#include "MotorControl.hpp"
#include "PID.h"

constexpr uint16_t CONTROL_WA = 0x200;

class Control : public chibios_rt::BaseStaticThread<CONTROL_WA>,
                public chibios_rt::EventListener{
public:
    static Control * instance();

    void main() override;

    void setCurrentGoal(Goal goal);

    Goal getCurrentGoal();

    void setAngleKp(float kp);

    void setDistanceKp(float kp);

    void setMotorPID(Board::IO::motor motor, float p, float i, float d);

    void reset();

private:
    explicit Control();

    void updateState();

    void applyControl();

    void updateDataStreamer();

    MotorControl m_motorControl;
    Goal      m_currentGoal;
    RobotPose m_robotPose;

    PID m_distancePID;
    PID m_anglePID;

    float m_linearSpeed;
    float m_linearSpeedSetpoint;
    float m_angularSpeed;
    float m_angularSpeedSetpoint;
    float m_angleSetpoint;
    float m_lastAngleSetpoint;
    float m_distanceError;
    bool m_forwardDrive;
    bool m_computeDirection;

};
