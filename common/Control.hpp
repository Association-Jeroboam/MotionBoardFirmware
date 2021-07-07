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

    void updateState(float dl, float dr, float left_speed, float right_speed);

    void applyControl(float dl, float dr);

    void setCurrentGoal(Goal goal);

    Goal getCurrentGoal();

    RobotPose getRobotPose();

    void setAnglePID(float kp, float ki, float kd);
    void setDistancePID(float kp, float ki, float kd);

    void setAngularSpeedPID(float kp, float ki, float kd);
    void setLinearSpeedPID(float kp, float ki, float kd);

    void setMotorPID(Peripherals::Motor motor, float p, float i, float d);

    void reset();

    ControlData getData();

  private:
    MotorControl m_motorControl;
    Goal         m_currentGoal;
    RobotPose    m_robotPose;

    PID m_distancePID;
    PID m_anglePID;

    PID m_linear_velPID;
    PID m_angular_velPID;

    float m_linearSpeed;
    float m_linearSpeedSetpoint;
    float m_lastLinearSpeedSetpoint;

    float m_angularSpeed;
    float m_angularSpeedSetpoint;
    float m_lastAngularSpeedSetpoint;

    float m_angleSetpoint;
    float m_lastAngleSetpoint;

    float m_distanceSetpoint;
    float m_lastDistanceSetpoint;

    float m_currentAngle;
    float m_currentDistance;
};
