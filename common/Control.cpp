#include "Control.hpp"
#include "LocalMath.hpp"
#include "Logging.hpp"
#include "MotionBoard.hpp"
#include "Parameters.hpp"
#include "Peripherals.hpp"
#include "RobotConf.hpp"
#include <cmath>

constexpr float circularTimeout = 2.;

Control::Control() : m_robotPose(INITIAL_X_POS, INITIAL_Y_POS, INITIAL_ANGLE),
m_linearController(0.1f, 0.f, MAX_WHEEL_SPEED, MAX_WHEEL_SPEED, MOTOR_CONTROL_LOOP_FREQ),
m_angularController(0.1f, 0.f, MAX_ANGULAR_SPEED, MAX_ANGULAR_SPEED, MOTOR_CONTROL_LOOP_FREQ) {
    m_linearSpeed          = 0.;
    m_linearSpeedSetpoint  = 0.;
    m_angularSpeed         = 0.;
    m_angularSpeedSetpoint = 0.;
    m_angleSetpoint        = INITIAL_ANGLE;
    m_lastAngleSetpoint    = INITIAL_ANGLE;
    m_distanceError        = 0.;
    m_computeDirection     = true;
    m_triggerCounter = 0;
    m_emergencyStop = Board::IO::getEmgcyState();
//    0.00030000
//    0.00060000
    // TODO : change me
    kP = KP;
    kA = KA;
    kB = KB; // Needs to be negative
}

void Control::update() {
    if(m_triggerCounter * MOTOR_CONTROL_LOOP_DT > CONTROL_COMMAND_TIMEOUT_S &&
        m_currentGoal.getType() != Goal::NO_GOAL && 
        m_currentGoal.getType() != Goal::PWM) {
        Goal noGoal = Goal();
        setCurrentGoal(noGoal);
        Logging::println("Goal timeout");
    }
    m_triggerCounter++;
    updateState();
    applyControl();
}

void Control::updateState() {
    m_dl = m_motorControl.getMotorDrivenDistance(Peripherals::LEFT_MOTOR);
    m_dr = m_motorControl.getMotorDrivenDistance(Peripherals::RIGHT_MOTOR);

    float left_speed  = m_motorControl.getMotorSpeed(Peripherals::LEFT_MOTOR);
    float right_speed = m_motorControl.getMotorSpeed(Peripherals::RIGHT_MOTOR);

    m_robotPose.update(m_dl, m_dr);
    m_linearSpeed  = (right_speed + left_speed) * 0.5;
    m_angularSpeed = (right_speed - left_speed) / m_robotPose.getWheelBase();
}

void Control::goToPose() {
    Goal::coordData_t goalData = m_currentGoal.getCoordData();

    m_angleSetpoint = goalData.theta;
    bool forwardMovementOnly = goalData.forwardMovementOnly;

    float currentTheta = m_robotPose.getModuloAngle();

    float diffX =  goalData.x - m_robotPose.getX();
    float diffY = goalData.y - m_robotPose.getY();
    float m_distanceError = sqrtf(pow(diffX, 2) + pow(diffY, 2));
    float m_angularError = normalizePi(m_robotPose.getModuloAngle() - goalData.theta);


    // End condition
    if (m_distanceError < DISTANCE_PRECISION && fabs(m_angularError) < ANGLE_PRECISION) {
        m_currentGoal.setReached(true);
        return;
    }

    float goalHeading = atan2f(diffY, diffX);
    float a = -currentTheta + goalHeading;
    float b = -m_angularError - a;

    int direction = 1;
    if (forwardMovementOnly) {
        a = normalizePi(a);
        b = normalizePi(b);
    } else {
        direction = sign<float>(cosf(a));
        a = normalizeHalfPi(a);
        b = normalizeHalfPi(b);
    }

    if (fabs(m_distanceError) < DISTANCE_PRECISION) {
        m_linearSpeedSetpoint = 0;
        m_angularSpeedSetpoint = kB * m_angularError;
    } else {
        m_linearSpeedSetpoint = kP * m_distanceError * direction;
        m_angularSpeedSetpoint = kA * a + kB * b;
    }

    if (fabs(m_linearSpeedSetpoint) > MAX_WHEEL_SPEED) {
        float ratio = MAX_WHEEL_SPEED / fabs(m_linearSpeedSetpoint);
        m_linearSpeedSetpoint *= ratio;
        m_angularSpeedSetpoint *= ratio;
    }

    if (fabs(m_angularSpeedSetpoint) > MAX_ANGULAR_SPEED) {
        float ratio = MAX_ANGULAR_SPEED / fabs(m_angularSpeedSetpoint);
        m_linearSpeedSetpoint *= ratio;
        m_angularSpeedSetpoint *= ratio;
    }

    if(fabs(m_distanceError) < DISTANCE_PRECISION && fabs(m_angularError) < ANGLE_PRECISION) {
        m_currentGoal.setReached(true);
    }

    
}

void Control::applyControl() {
    float leftSpeedSetpoint;
    float rightSpeedSetpoint;

//    float lastLinearSpeedSetpoint = m_linearSpeedSetpoint;
//    float lastAngularSpeedSetpoint = m_angularSpeedSetpoint;
//    float linearAccl;
//    float angularAccl;

    switch (m_currentGoal.getType()) {
        case Goal::COORD: {
            m_motorControl.setDisable(false);
            goToPose();
            break;
        }
        case Goal::CIRCULAR: {
            t += MOTOR_CONTROL_LOOP_DT;
            m_angularSpeedSetpoint = m_currentGoal.getCircularData().angularSpeed;
            m_linearSpeedSetpoint  = m_currentGoal.getCircularData().linearSpeed;

            m_linearController.setSpeedGoal(m_linearSpeedSetpoint);
            m_linearSpeedSetpoint += m_linearController.update(m_linearSpeed);

            m_angularController.setSpeedGoal(m_angularSpeedSetpoint);
            m_angularSpeedSetpoint += m_angularController.update(m_angularSpeed);

            m_motorControl.setDisable(false);
            break;
        }
        case Goal::SPEED: {
            leftSpeedSetpoint  = m_currentGoal.getSpeedData().leftSpeed;
            rightSpeedSetpoint = m_currentGoal.getSpeedData().rightSpeed;
            m_motorControl.setDisable(false);
            goto set_speeds;
        }
        case Goal::PWM: {
            Board::IO::setMotorDutyCycle(Peripherals::Motor::LEFT_MOTOR, m_currentGoal.getPWMData().leftPWM);
            Board::IO::setMotorDutyCycle(Peripherals::Motor::RIGHT_MOTOR, m_currentGoal.getPWMData().rightPWM);
            m_motorControl.setDisable(false);
            m_motorControl.updateMeasure();
            return;
        }
        case Goal::NO_GOAL: {
            m_linearSpeedSetpoint  = 0.;
            m_angularSpeedSetpoint = 0.;
            m_angleSetpoint        = 0.;
            m_distanceError        = 0.;
            m_currentGoal.setReached(true);
            m_motorControl.resetMotor(Peripherals::LEFT_MOTOR);
            m_motorControl.resetMotor(Peripherals::RIGHT_MOTOR);
            m_motorControl.setDisable(true);
            goto update;
        }
        case Goal::DISTANCE:
            break;
        case Goal::ANGLE:
            break;
    }

//    linearAccl = (m_linearSpeedSetpoint - m_linearSpeed)/MOTOR_CONTROL_LOOP_DT;
//    if (linearAccl > MAX_LINEAR_ACCL) {
//        m_linearSpeedSetpoint = m_linearSpeed + MAX_LINEAR_ACCL * MOTOR_CONTROL_LOOP_DT;
//    } else if (linearAccl < -MAX_LINEAR_ACCL) {
//        m_linearSpeedSetpoint = m_linearSpeed - MAX_LINEAR_ACCL * MOTOR_CONTROL_LOOP_DT;
//    }
//    angularAccl = (m_angularSpeedSetpoint - m_angularSpeed)/MOTOR_CONTROL_LOOP_DT;
//    if (angularAccl > MAX_ANGULAR_ACCL) {
//        m_angularSpeedSetpoint = m_angularSpeed + MAX_ANGULAR_ACCL * MOTOR_CONTROL_LOOP_DT;
//    } else if (angularAccl < -MAX_ANGULAR_ACCL) {
//        m_angularSpeedSetpoint = m_angularSpeed - MAX_ANGULAR_ACCL * MOTOR_CONTROL_LOOP_DT;
//    }


    leftSpeedSetpoint  = m_linearSpeedSetpoint - m_angularSpeedSetpoint * m_robotPose.getWheelBase() * 0.5;
    rightSpeedSetpoint = m_linearSpeedSetpoint + m_angularSpeedSetpoint * m_robotPose.getWheelBase() * 0.5;

set_speeds:
    if(m_currentGoal.isReached()) {
        // m_motorControl.motorSetSpeed(Peripherals::LEFT_MOTOR, 0.);
        // m_motorControl.motorSetSpeed(Peripherals::RIGHT_MOTOR, 0.);
        m_motorControl.resetMotor(Peripherals::LEFT_MOTOR);
        m_motorControl.resetMotor(Peripherals::RIGHT_MOTOR);
    } else {
        m_motorControl.motorSetSpeed(Peripherals::LEFT_MOTOR, leftSpeedSetpoint);
        m_motorControl.motorSetSpeed(Peripherals::RIGHT_MOTOR, rightSpeedSetpoint);
    }
update:
    m_motorControl.update();
}

void Control::setCurrentGoal(Goal goal) {
    m_currentGoal      = goal;
    m_computeDirection = true;

    Goal::GoalType goalType     = m_currentGoal.getType();
    if(goalType == Goal::GoalType::COORD    ||
       goalType == Goal::GoalType::DISTANCE ||
       goalType == Goal::GoalType::ANGLE      ) {

        m_motorControl.resetMotor(Peripherals::LEFT_MOTOR);
        m_motorControl.resetMotor(Peripherals::RIGHT_MOTOR);

    }
    // timeout on command, better be done specifically for circular goals, but meh ¯\_(ツ)_/¯
    m_triggerCounter = 0;

    t = 0;
}

Goal Control::getCurrentGoal() {
    return m_currentGoal;
}

void Control::setMotorPID(Peripherals::Motor motor, float p, float i, float d) {
    m_motorControl.motorSetPID(motor, p, i, d);
}

void Control::setPID(float kP, float kA, float kB) {
    this->kP = kP;
    this->kA = kA;
    this->kB = kB;
}

void Control::reset() {
    m_robotPose.setPose(INITIAL_X_POS, INITIAL_Y_POS, INITIAL_ANGLE);
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
    m_distance = 0;
    m_dl = 0;
    m_dr = 0;
    t = 0;
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
    // TODO : add angularError
    data.x                    = m_robotPose.getX();
    data.y                    = m_robotPose.getY();
    if(m_currentGoal.getType() == Goal::COORD) {
        data.xSetpoint            = m_currentGoal.getCoordData().x;
        data.ySetpoint            = m_currentGoal.getCoordData().y;
    } else {
        data.xSetpoint = 0;
        data.ySetpoint = 0;
    }


    return data;
}

RobotPose* Control::getRobotPose() {
    return &m_robotPose;
}

void Control::setEmergency(bool emergency) {
    if(m_emergencyStop != emergency) {
        m_motorControl.setDisable(emergency);
        m_emergencyStop = emergency;
    }
}

void Control::getMotorTicks(int32_t * left, int32_t * right) {
    *left  = m_motorControl.getMotorEncoderTicks(Peripherals::LEFT_MOTOR);
    *right = m_motorControl.getMotorEncoderTicks(Peripherals::RIGHT_MOTOR);
}

SpeedControllerParameters Control::getLinearControllerParams() {
    return m_linearController.getParameters();
}

SpeedControllerParameters Control::getAngularControllerParams() {
    return m_angularController.getParameters();
}
