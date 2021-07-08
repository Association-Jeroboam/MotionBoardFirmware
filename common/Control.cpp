#include "Control.hpp"
#include "LocalMath.hpp"
#include "Logging.hpp"
#include "MotionBoard.hpp"
#include "Parameters.hpp"
#include "Peripherals.hpp"
#include <cmath>

constexpr float circularTimeout = 2.;

Control::Control() : m_robotPose(INITIAL_X_POS, INITIAL_Y_POS, INITIAL_ANGLE) {
    m_distancePID.set(DISTANCE_KP, DISTANCE_KI, 0., 0., MOTOR_CONTROL_LOOP_FREQ);
    m_anglePID.set(ANGLE_KP, 0., 0., 0., MOTOR_CONTROL_LOOP_FREQ);
    m_distancePID.setMaxOutput(MAX_WHEEL_SPEED);
    m_distancePID.setMaxIntegral(MAX_WHEEL_SPEED);
    m_anglePID.setMaxIntegral(MAX_ANGULAR_SPEED);
    m_anglePID.setMaxOutput(MAX_ANGULAR_SPEED);
    m_linearSpeed          = 0.;
    m_linearSpeedSetpoint  = 0.;
    m_angularSpeed         = 0.;
    m_angularSpeedSetpoint = 0.;
    m_angleSetpoint        = INITIAL_ANGLE;
    m_lastAngleSetpoint    = INITIAL_ANGLE;
    m_distanceError        = 0.;
    m_computeDirection     = true;
}

void Control::update() {
    updateState();
    applyControl();
}

void Control::updateState() {
    float dl = m_motorControl.getMotorDrivenDistance(Peripherals::LEFT_MOTOR);
    float dr = m_motorControl.getMotorDrivenDistance(Peripherals::RIGHT_MOTOR);

    float left_speed  = m_motorControl.getMotorSpeed(Peripherals::LEFT_MOTOR);
    float right_speed = m_motorControl.getMotorSpeed(Peripherals::RIGHT_MOTOR);

    m_robotPose.update(dl, dr);
    m_linearSpeed  = (right_speed + left_speed) * 0.5;
    m_angularSpeed = (right_speed - left_speed) / WHEEL_BASE;
}

void Control::applyControl() {
    float leftSpeedSetpoint;
    float rightSpeedSetpoint;

    float lastLinearSpeedSetpoint = m_linearSpeedSetpoint;
    float lastAngularSpeedSetpoint = m_angularSpeedSetpoint;
    float linearAccl;
    float angularAccl;
    m_motorControl.setDisable(m_emergencyStop);

    switch (m_currentGoal.getType()) {
        case Goal::ANGLE: {
            t += MOTOR_CONTROL_LOOP_DT;
            float finalAngle        = (m_currentGoal.getAngleData().angle + m_currentGoal.getAngleData().turns * 2 * M_PI);
            float rotDir;
            float a;
            float b;
            if(finalAngle >= initialPos) {
                rotDir = 1.;
                a = finalAngle;
                b = initialPos;
            } else {
                rotDir = -1;
                a = initialPos;
                b = finalAngle;
            }


            float coeff = 1 / (1 + expf(-rotDir * 4.*(t-2.5)));
            m_angleSetpoint        = (a - b) * coeff + b;

            m_angularSpeedSetpoint = m_anglePID.compute(m_angleSetpoint, m_robotPose.getAbsoluteAngle());
            m_linearSpeedSetpoint  = 0.;

            if (fabs(finalAngle - m_robotPose.getAbsoluteAngle()) < ANGLE_PRECISION && !m_currentGoal.isReached()) {
                m_currentGoal.setReached(true);
            }

            break;
        }
        case Goal::COORD: {
            float xError      = m_currentGoal.getCoordData().x - m_robotPose.getX();
            float yError      = m_currentGoal.getCoordData().y - m_robotPose.getY();
            float angleToGoal = atan2(yError, xError);
            float angleError = angleToGoal - m_robotPose.getModuloAngle();
            angleError = moduloTwoPI(angleError);

            if (m_computeDirection) {
                switch (m_currentGoal.getCoordData().direction) {
                    case Goal::FORWARD:
                        m_forwardDrive = true;
                        break;
                    case Goal::BACKWARD:
                        m_forwardDrive = false;
                        break;
                    case Goal::ANY:
                        float angleError = angleToGoal - m_robotPose.getModuloAngle();
                        angleError       = moduloTwoPI(angleError);
                        if (-M_PI * 0.5 < angleError && angleError < M_PI * 0.5) {
                            m_forwardDrive = true;
                        } else {
                            m_forwardDrive = false;
                        }
                        break;
                }
                m_computeDirection = false;
            }
            angleToGoal = angleError + m_robotPose.getAbsoluteAngle();

            if (m_forwardDrive) {
                m_angleSetpoint = angleToGoal;
                m_distanceError = sqrtf(xError * xError + yError * yError);
            } else {
               if (angleToGoal > 0) {
                   m_angleSetpoint = -M_PI;
               } else {
                   m_angleSetpoint = M_PI;
               }
               m_angleSetpoint += angleToGoal;
               m_distanceError = -sqrtf(xError * xError + yError * yError);

            }

            if (fabs(m_distanceError) < DISTANCE_PRECISION && !m_currentGoal.isReached()) {
                m_currentGoal.setReached(true);
            }

            m_angularSpeedSetpoint = m_anglePID.compute(m_angleSetpoint, m_robotPose.getAbsoluteAngle());
            m_lastAngleSetpoint    = m_angleSetpoint;
            //prevents motor saturation
            m_distancePID.setMaxOutput(MAX_WHEEL_SPEED);
            float maxLinearSpeed = MAX_WHEEL_SPEED - fabsf(m_angularSpeedSetpoint) * WHEEL_BASE * 0.5;

            m_linearSpeedSetpoint = m_distancePID.compute(m_distanceError, 0.);

            if (m_distanceError < COMPUTE_DIRECTION_THRESHOLD) {
                m_computeDirection = true;
                m_currentGoal.setCoordDirection(Goal::ANY);
            }
            break;
        }
        case Goal::CIRCULAR: {
            t += MOTOR_CONTROL_LOOP_DT;
            m_angularSpeedSetpoint = m_currentGoal.getCircularData().angularSpeed;
            m_linearSpeedSetpoint  = m_currentGoal.getCircularData().linearSpeed;
            if ( t > circularTimeout ){
                m_currentGoal.setReached(true);
            }
            break;
        }
        case Goal::SPEED: {
            leftSpeedSetpoint  = m_currentGoal.getSpeedData().leftSpeed;
            rightSpeedSetpoint = m_currentGoal.getSpeedData().rightSpeed;
            goto set_speeds;
        }
        case Goal::PWM: {
            Board::IO::setMotorDutyCycle(Peripherals::Motor::LEFT_MOTOR, m_currentGoal.getPWMData().leftPWM);
            Board::IO::setMotorDutyCycle(Peripherals::Motor::RIGHT_MOTOR, m_currentGoal.getPWMData().rightPWM);
            break;
        }
        case Goal::NO_GOAL: {
            m_linearSpeed          = 0.;
            m_linearSpeedSetpoint  = 0.;
            m_angularSpeed         = 0.;
            m_angularSpeedSetpoint = 0.;
            m_angleSetpoint        = 0.;
            m_distanceError        = 0.;
            m_currentGoal.setReached(true);
            m_motorControl.resetMotor(Peripherals::LEFT_MOTOR);
            m_motorControl.resetMotor(Peripherals::RIGHT_MOTOR);
            break;
        }
    }

    linearAccl = (m_linearSpeedSetpoint - lastLinearSpeedSetpoint)/MOTOR_CONTROL_LOOP_DT;
    if (linearAccl > MAX_LINEAR_ACCL) {
        m_linearSpeedSetpoint = lastLinearSpeedSetpoint + MAX_LINEAR_ACCL * MOTOR_CONTROL_LOOP_DT;
    }
//    angularAccl = (m_angularSpeedSetpoint - lastAngularSpeedSetpoint)/MOTOR_CONTROL_LOOP_DT;
//    if (angularAccl > MAX_ANGULAR_ACCL) {
//        m_angularSpeedSetpoint = lastAngularSpeedSetpoint + MAX_ANGULAR_ACCL * MOTOR_CONTROL_LOOP_DT;
//    } else if (angularAccl < -MAX_ANGULAR_ACCL) {
//        m_angularSpeedSetpoint = lastAngularSpeedSetpoint - MAX_ANGULAR_ACCL * MOTOR_CONTROL_LOOP_DT;
//    }


    leftSpeedSetpoint  = m_linearSpeedSetpoint - m_angularSpeedSetpoint * WHEEL_BASE * 0.5;
    rightSpeedSetpoint = m_linearSpeedSetpoint + m_angularSpeedSetpoint * WHEEL_BASE * 0.5;

set_speeds:
    if(m_currentGoal.isReached() || m_emergencyStop) {
        m_motorControl.motorSetSpeed(Peripherals::LEFT_MOTOR, 0.);
        m_motorControl.motorSetSpeed(Peripherals::RIGHT_MOTOR, 0.);
        m_motorControl.resetMotor(Peripherals::LEFT_MOTOR);
        m_motorControl.resetMotor(Peripherals::RIGHT_MOTOR);
    } else {
        m_motorControl.motorSetSpeed(Peripherals::LEFT_MOTOR, leftSpeedSetpoint);
        m_motorControl.motorSetSpeed(Peripherals::RIGHT_MOTOR, rightSpeedSetpoint);
    }

    m_motorControl.update();
}

void Control::setCurrentGoal(Goal goal) {
    m_currentGoal      = goal;
    m_computeDirection = true;
    m_currentGoal.print();
    m_motorControl.resetMotor(Peripherals::LEFT_MOTOR);
    m_motorControl.resetMotor(Peripherals::RIGHT_MOTOR);
    m_distancePID.reset();
    m_anglePID.reset();
    Goal::GoalType goalType = m_currentGoal.getType();

    t = 0;
    if (goalType == Goal::ANGLE) {
        m_anglePID.set(ANGLE_KP, 0., 0., 0., MOTOR_CONTROL_LOOP_FREQ);
        float goalValue = m_currentGoal.getAngleData().angle + m_currentGoal.getAngleData().turns * 2 * M_PI;
        float initialValue = m_robotPose.getAbsoluteAngle();

        if (initialValue <= goalValue) {
            direction = 1;
        } else {
            direction = -1;
        }
        initialPos = m_robotPose.getAbsoluteAngle();
        goalPos = fabsf(initialPos - goalValue);

    } else if (goalType == Goal::COORD) {
        m_anglePID.set(5., 0., 0., 0., MOTOR_CONTROL_LOOP_FREQ);
    }
    // else if (goalType == Goal::DISTANCE) {
    //     computePeriods(distancePeak, T);
    // }
}

Goal Control::getCurrentGoal() {
    return m_currentGoal;
}

void Control::setAngleKp(float kp) {
    m_anglePID.set(kp, 0., 0.);
    m_motorControl.resetMotor(Peripherals::LEFT_MOTOR);
    m_motorControl.resetMotor(Peripherals::RIGHT_MOTOR);
}

void Control::setDistanceKp(float kp) {
    m_distancePID.set(kp, DISTANCE_KI, 0.);
    m_motorControl.resetMotor(Peripherals::LEFT_MOTOR);
    m_motorControl.resetMotor(Peripherals::RIGHT_MOTOR);
}

void Control::setMotorPID(Peripherals::Motor motor, float p, float i, float d) {
    m_motorControl.motorSetPID(motor, p, i, d);
}

void Control::reset() {
    m_robotPose.setPose(INITIAL_X_POS, INITIAL_Y_POS, INITIAL_ANGLE);
    m_anglePID.reset();
    m_distancePID.reset();
    m_motorControl.resetMotor(Peripherals::LEFT_MOTOR);
    m_motorControl.resetMotor(Peripherals::RIGHT_MOTOR);
    m_linearSpeed          = 0.;
    m_linearSpeedSetpoint  = 0.;
    m_angularSpeed         = 0.;
    m_angularSpeedSetpoint = 0.;
    m_angleSetpoint        = INITIAL_ANGLE;
    m_lastAngleSetpoint    = INITIAL_ANGLE;
    m_distanceError        = 0.;
    m_currentGoal          = Goal();
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
    data.distanceError        = m_distanceError;
    data.x                    = m_robotPose.getX();
    data.y                    = m_robotPose.getY();

    return data;
}

RobotPose* Control::getRobotPose() {
    return &m_robotPose;
}

void Control::setEmergency(bool emergency) {

    m_emergencyStop = emergency;
    Logging::println("emgy %u", m_emergencyStop);
    m_anglePID.reset();
    m_distancePID.reset();
}
