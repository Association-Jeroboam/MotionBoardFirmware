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
    float xSetpoint;
    float y;
    float ySetpoint;
};

class Control {
  public:
    Control();

    void update();

    void updateState();

    void applyControl();

    void setCurrentGoal(Goal goal);

    Goal getCurrentGoal();

    RobotPose* getRobotPose();

    void setMotorPID(Peripherals::Motor motor, float p, float i, float d);

    void setPID(float kP, float kA, float kB);

    void reset();

    void setEmergency(bool emergency);

    inline bool getEmergency() { return m_emergencyStop;};

    ControlData getData();

    RobotPose    m_robotPose;

  private:
    void goToPose();

    MotorControl m_motorControl;
    Goal         m_currentGoal;

    float goalPos;
    bool m_emergencyStop;

    float direction    = 1;
    float initialPos   = 0;
    float t            = 0;

    float m_distance;
    float m_dl;
    float m_dr;

    float m_linearSpeed;
    float m_linearSpeedSetpoint;
    float m_angularSpeed;
    float m_angularSpeedSetpoint;
    float m_angleSetpoint;
    float m_lastAngleSetpoint;
    float m_distanceError;
    bool  m_forwardDrive;
    bool  m_computeDirection;
    float m_angularError;

    float kA;
    float kB;
    float kP;
};
