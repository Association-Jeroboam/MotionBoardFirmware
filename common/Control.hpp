#pragma once

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
