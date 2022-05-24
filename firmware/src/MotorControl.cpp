#include "MotorControl.hpp"
#include "DataStreamer.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"
#include "Peripherals.hpp"

MotorControl::MotorControl() : m_leftMotor(Peripherals::LEFT_ENCODER, Peripherals::LEFT_MOTOR, WHEEL_LEFT_RADIUS),
                               m_rightMotor(Peripherals::RIGHT_ENCODER, Peripherals::RIGHT_MOTOR, WHEEL_RIGHT_RADIUS) {
    motorSetSpeed(Peripherals::LEFT_MOTOR, 0.);
    motorSetSpeed(Peripherals::RIGHT_MOTOR, 0.);
}

void MotorControl::update() {

    m_leftMotor.updateControl();
    m_rightMotor.updateControl();
    DataStreamer::instance()->setEntry(leftWheelSpeedEnum, m_leftMotor.getSpeed());
    DataStreamer::instance()->setEntry(rightWheelSpeedEnum, m_rightMotor.getSpeed());
}

void MotorControl::motorSetSpeed(Peripherals::Motor motor, float speed) {
    switch (motor) {
        case Peripherals::LEFT_MOTOR:
            m_leftMotor.setSpeed(speed);
            DataStreamer::instance()->setEntry(leftWheelSpeedSetpointEnum, speed);
            break;
        case Peripherals::RIGHT_MOTOR:
            m_rightMotor.setSpeed(speed);
            DataStreamer::instance()->setEntry(rightWheelSpeedSetpointEnum, speed);
            break;
    }
}

void MotorControl::motorSetPID(Peripherals::Motor motor, float p, float i, uint8_t range) {
    switch (motor) {
        case Peripherals::LEFT_MOTOR:
            m_leftMotor.setPID(p, i, range);
            break;
        case Peripherals::RIGHT_MOTOR:
            m_rightMotor.setPID(p, i, range);
            break;
    }
}

void MotorControl::motorSetPIDThreshold(Peripherals::Motor motor, float threshold, uint8_t range) {
    switch (motor) {
        case Peripherals::LEFT_MOTOR:
            m_leftMotor.setPIDThreshold(threshold, range);
            break;
        case Peripherals::RIGHT_MOTOR:
            m_rightMotor.setPIDThreshold(threshold, range);
            break;
    }
}

void MotorControl::setWheelRadius(Peripherals::Motor motor, float wheelRadius) {
    switch (motor) {
        case Peripherals::LEFT_MOTOR:
            m_leftMotor.setWheelRadius(wheelRadius);
            break;
        case Peripherals::RIGHT_MOTOR:
            m_rightMotor.setWheelRadius(wheelRadius);
            break;
    }
}

float MotorControl::getMotorDrivenDistance(Peripherals::Motor motor) {
    float drivenDistance;
    switch (motor) {
        case Peripherals::LEFT_MOTOR:
            drivenDistance = m_leftMotor.getDrivenDistance();
            break;
        case Peripherals::RIGHT_MOTOR:
            drivenDistance = m_rightMotor.getDrivenDistance();
            break;
    }
    return drivenDistance;
}

float MotorControl::getMotorSpeed(Peripherals::Motor motor) {
    float speed;
    switch (motor) {
        case Peripherals::LEFT_MOTOR:
            speed = m_leftMotor.getSpeed();
            break;
        case Peripherals::RIGHT_MOTOR:
            speed = m_rightMotor.getSpeed();
            break;
    }
    return speed;
}

SpeedControllerParameters MotorControl::getMotorControllerParameters(Peripherals::Motor motor){
    SpeedControllerParameters params;
    switch (motor) {
        case Peripherals::LEFT_MOTOR:
            params = m_leftMotor.getControllerParameters();
            break;
        case Peripherals::RIGHT_MOTOR:
            params = m_rightMotor.getControllerParameters();
            break;
    }
    return params;

}

void MotorControl::resetMotor(Peripherals::Motor motor) {
    switch (motor) {
        case Peripherals::LEFT_MOTOR:
            m_leftMotor.reset();
            break;
        case Peripherals::RIGHT_MOTOR:
            m_rightMotor.reset();
            break;
    }
}

void MotorControl::setDisable(bool disable){
    m_leftMotor.setDisable(disable);
    m_rightMotor.setDisable(disable);
}