#include "Control.hpp"
#include "LocalMath.hpp"
#include "Logging.hpp"
#include "MotionBoard.hpp"
#include "Parameters.hpp"
#include "Peripherals.hpp"
#include <cmath>

Control::Control() : m_robotPose(INITIAL_X_POS, INITIAL_Y_POS, INITIAL_ANGLE) {
    m_distancePID.set(1, 0., 0., 0., MOTOR_CONTROL_LOOP_FREQ);
    m_distancePID.setMaxOutput(MAX_WHEEL_SPEED);

    m_anglePID.set(1, 0., 0., 0., MOTOR_CONTROL_LOOP_FREQ);
    m_anglePID.setMaxOutput(MAX_ANGULAR_SPEED);

    m_linear_velPID.set(0, 0., 0., 0., MOTOR_CONTROL_LOOP_FREQ);
    m_linear_velPID.setMaxOutput(DEFAULT_MAX_PID_OUTPUT);

    m_angular_velPID.set(0, 0., 0., 0., MOTOR_CONTROL_LOOP_FREQ);
    m_angular_velPID.setMaxOutput(DEFAULT_MAX_PID_OUTPUT);

    reset();
}

void Control::updateState(float dl, float dr, float left_speed, float right_speed) {
    m_robotPose.update(dl, dr);
    m_linearSpeed  = (right_speed + left_speed) * 0.5;
    m_angularSpeed = (right_speed - left_speed) / WHEEL_BASE;
}

void Control::update() {
    // Odometry
    m_motorControl.updateMeasure();

    float dl = m_motorControl.getMotorDrivenDistance(Peripherals::LEFT_MOTOR);
    float dr = m_motorControl.getMotorDrivenDistance(Peripherals::RIGHT_MOTOR);

    float left_speed  = m_motorControl.getMotorSpeed(Peripherals::LEFT_MOTOR);
    float right_speed = m_motorControl.getMotorSpeed(Peripherals::RIGHT_MOTOR);

    updateState(dl, dr, left_speed, right_speed);

    // Control
    applyControl(dl, dr);
}

void Control::applyControl(float dl, float dr) {
    float leftSpeedSetpoint;
    float rightSpeedSetpoint;

    m_lastLinearSpeedSetpoint  = m_linearSpeedSetpoint;
    m_lastAngularSpeedSetpoint = m_angularSpeedSetpoint;

    m_lastDistanceSetpoint = m_distanceSetpoint;
    m_lastAngleSetpoint    = m_angleSetpoint;

    switch (m_currentGoal.getType()) {
        case Goal::POLAR: {
            Goal::polarData_t goalData = m_currentGoal.getPolarData();

            // Compute angular speed setpoint
            m_currentAngle += (dr - dl) / WHEEL_BASE;
            m_angleSetpoint = goalData.angle + goalData.turns * 2 * M_PI;

            m_angularSpeedSetpoint = m_anglePID.compute(m_angleSetpoint, m_currentAngle);

            // Compute linear speed setpoint
            m_currentDistance     = (dr + dl) * 0.5;
            m_distanceSetpoint    = goalData.distance;
            m_linearSpeedSetpoint = m_anglePID.compute(m_distanceSetpoint, m_currentDistance);

            if ((fabs(angleDifference(m_angleSetpoint, m_currentAngle)) < ANGLE_PRECISION) && (m_distanceSetpoint - m_currentDistance < DISTANCE_PRECISION) && !m_currentGoal.isReached()) {
                m_currentGoal.setReached(true);
            }

            break;
        }
        case Goal::CIRCULAR: {
            Goal::circularData_t goalData = m_currentGoal.getCircularData();

            m_angularSpeedSetpoint = goalData.angularSpeed;
            m_linearSpeedSetpoint  = goalData.linearSpeed;

            m_currentGoal.setReached(true);

            break;
        }
        case Goal::PWM: {
            m_motorControl.motorSetSpeed(Peripherals::Motor::LEFT_MOTOR, m_currentGoal.getPWMData().leftPWM);
            m_motorControl.motorSetSpeed(Peripherals::Motor::RIGHT_MOTOR, m_currentGoal.getPWMData().rightPWM);

            m_currentGoal.setReached(true);

            break;
        }
        case Goal::NO_GOAL: {
            m_linearSpeed             = 0.;
            m_linearSpeedSetpoint     = 0.;
            m_lastLinearSpeedSetpoint = 0.;

            m_angularSpeed             = 0.;
            m_angularSpeedSetpoint     = 0.;
            m_lastAngularSpeedSetpoint = 0.;

            m_angleSetpoint     = 0.;
            m_lastAngleSetpoint = 0.;

            m_currentGoal.setReached(true);

            m_motorControl.resetMotor(Peripherals::LEFT_MOTOR);
            m_motorControl.resetMotor(Peripherals::RIGHT_MOTOR);

            break;
        }
    }

    // Enforce acceleration limits
    if (fabs(angleDifference(m_angularSpeedSetpoint, m_lastAngularSpeedSetpoint)) / MOTOR_CONTROL_LOOP_DT > MAX_ANGULAR_ACCEL) {
        m_angularSpeedSetpoint = m_angularSpeedSetpoint + sign<float>(m_angularSpeedSetpoint) * MAX_ANGULAR_ACCEL * MOTOR_CONTROL_LOOP_DT;
    }

    if (fabs(m_linearSpeedSetpoint - m_lastLinearSpeedSetpoint) / MOTOR_CONTROL_LOOP_DT > MAX_LINEAR_ACCEL) {
        m_linearSpeedSetpoint = m_linearSpeedSetpoint + sign<float>(m_linearSpeedSetpoint) * MAX_LINEAR_ACCEL * MOTOR_CONTROL_LOOP_DT;
    }

    // Enforce speed limits
    if (fabs(m_angularSpeedSetpoint) > MAX_ANGULAR_SPEED) {
        m_angularSpeedSetpoint = sign<float>(m_angularSpeedSetpoint) * MAX_ANGULAR_SPEED;
    }

    if (fabs(m_linearSpeedSetpoint) > MAX_WHEEL_SPEED) {
        m_linearSpeedSetpoint = sign<float>(m_linearSpeedSetpoint) * MAX_WHEEL_SPEED;
    }

    leftSpeedSetpoint  = m_linearSpeedSetpoint - m_angularSpeedSetpoint * WHEEL_BASE * 0.5;
    rightSpeedSetpoint = m_linearSpeedSetpoint + m_angularSpeedSetpoint * WHEEL_BASE * 0.5;

    if (fabs(leftSpeedSetpoint) > MAX_WHEEL_SPEED) {
        leftSpeedSetpoint = sign<float>(leftSpeedSetpoint) * MAX_WHEEL_SPEED;
    }

    if (fabs(rightSpeedSetpoint) > MAX_WHEEL_SPEED) {
        rightSpeedSetpoint = sign<float>(rightSpeedSetpoint) * MAX_WHEEL_SPEED;
    }

    m_motorControl.motorSetSpeed(Peripherals::LEFT_MOTOR, leftSpeedSetpoint);
    m_motorControl.motorSetSpeed(Peripherals::RIGHT_MOTOR, rightSpeedSetpoint);
    m_motorControl.updateControl();
}

void Control::setCurrentGoal(Goal goal) {
    m_currentGoal = goal;
    m_currentGoal.print();

    if (m_currentGoal.getType() == Goal::POLAR) {
        // Reset angle and distance accumulator since goals are relative
        m_currentAngle    = 0;
        m_currentDistance = 0;
    }
}

Goal Control::getCurrentGoal() {
    return m_currentGoal;
}

void Control::setAnglePID(float kp, float ki, float kd) {
    m_anglePID.set(kp, ki, kd);
}

void Control::setDistancePID(float kp, float ki, float kd) {
    m_distancePID.set(kp, ki, kd);
}

void Control::setAngularSpeedPID(float kp, float ki, float kd) {
    m_angular_velPID.set(kp, ki, kd);
}

void Control::setLinearSpeedPID(float kp, float ki, float kd) {
    m_linear_velPID.set(kp, ki, kd);
}

void Control::reset() {
    m_robotPose.setPose(INITIAL_X_POS, INITIAL_Y_POS, INITIAL_ANGLE);

    m_anglePID.reset();
    m_distancePID.reset();

    m_motorControl.resetMotor(Peripherals::LEFT_MOTOR);
    m_motorControl.resetMotor(Peripherals::RIGHT_MOTOR);

    m_linearSpeed             = 0.;
    m_linearSpeedSetpoint     = 0.;
    m_lastLinearSpeedSetpoint = 0.;

    m_angularSpeed             = 0.;
    m_angularSpeedSetpoint     = 0.;
    m_lastAngularSpeedSetpoint = 0.;

    m_angleSetpoint     = INITIAL_ANGLE;
    m_lastAngleSetpoint = INITIAL_ANGLE;

    m_distanceSetpoint     = 0.;
    m_lastDistanceSetpoint = 0.;

    m_currentAngle    = 0.;
    m_currentDistance = 0.;

    m_currentGoal = Goal();
}

ControlData Control::getData() {
    ControlData data;
    data.angularSpeed         = m_angularSpeed;
    data.angularSpeedSetpoint = m_angularSpeedSetpoint;
    data.linearSpeed          = m_linearSpeed;
    data.linearSpeedSetpoint  = m_linearSpeedSetpoint;
    data.angle                = m_robotPose.getModuloAngle();
    data.absoluteAngle        = m_robotPose.getAbsoluteAngle();
    data.angleSetpoint        = m_angleSetpoint;
    data.x                    = m_robotPose.getX();
    data.y                    = m_robotPose.getY();

    return data;
}

RobotPose Control::getRobotPose() {
    return m_robotPose;
}
