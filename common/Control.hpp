#pragma once

#include "LocalMath.hpp"
#include "Goal.hpp"
#include "MotorControl.hpp"
#include "PID.h"
#include "RobotPose.hpp"

struct ControlData {
    float angularSpeed;
    float angularSpeedSetpoint;
    float linearSpeed;
    float linearSpeedSetpoint;
    float angle;
    float absoluteAngle;
    float angleSetpoint;
    float distanceError;
    float x;
    float y;
};

class Control {
  public:
    Control();

    void update();

    void updateState();

    void applyControl();

    void setCurrentGoal(Goal goal);

    Goal getCurrentGoal();

    RobotPose getRobotPose();

    void setAngleKp(float kp);

    void setDistanceKp(float kp);

    void setMotorPID(Peripherals::Motor motor, float p, float i, float d);

    void reset();

    ControlData getData();

  private:
    MotorControl m_motorControl;
    Goal         m_currentGoal;
    RobotPose    m_robotPose;

    PID m_distancePID;
    PID m_anglePID;

    float direction    = 1;
    float initialPos   = 0;
    float t            = 0;
    float anglePeak[4] = {0, M_PI, M_PI, M_PI};
    float T[4]         = {0, 0, 0, 0};

    float m_linearSpeed;
    float m_linearSpeedSetpoint;
    float m_angularSpeed;
    float m_angularSpeedSetpoint;
    float m_angleSetpoint;
    float m_lastAngleSetpoint;
    float m_distanceError;
    bool  m_forwardDrive;
    bool  m_computeDirection;
};
