#include "MotorControl.hpp"
#include "Parameters.hpp"
#include "DataStreamer.hpp"



using namespace Board::IO;

MotorControl::MotorControl() :
        m_leftMotor(LEFT_ENCODER, LEFT_MOTOR, WHEEL_LEFT_RADIUS),
        m_rightMotor(RIGHT_ENCODER, RIGHT_MOTOR, WHEEL_RIGHT_RADIUS) {
    m_leftMotor.setPID(LEFT_MOTOR_KP, LEFT_MOTOR_KI, LEFT_MOTOR_KD, LEFT_MOTOR_BIAS, MOTOR_CONTROL_LOOP_FREQ);
    m_rightMotor.setPID(RIGHT_MOTOR_KP, RIGHT_MOTOR_KI, RIGHT_MOTOR_KD, RIGHT_MOTOR_BIAS, MOTOR_CONTROL_LOOP_FREQ);
    motorSetSpeed(LEFT_MOTOR, 0.);
    motorSetSpeed(RIGHT_MOTOR, 0.);
}

void MotorControl::update() {

    m_leftMotor.updateControl();
    m_rightMotor.updateControl();
    DataStreamer::instance()->setEntry(leftWheelSpeedEnum, m_leftMotor.getSpeed());
    DataStreamer::instance()->setEntry(rightWheelSpeedEnum, m_rightMotor.getSpeed());
}

void MotorControl::motorSetSpeed(enum motor motor, float speed) {
    switch (motor) {
        case LEFT_MOTOR:
            m_leftMotor.setSpeed(speed);
            DataStreamer::instance()->setEntry(leftWheelSpeedSetpointEnum, speed);
            break;
        case RIGHT_MOTOR:
            m_rightMotor.setSpeed(speed);
            DataStreamer::instance()->setEntry(rightWheelSpeedSetpointEnum, speed);
            break;
    }
}

void MotorControl::motorSetPID(enum motor motor, float p, float i, float d) {
    switch (motor) {
        case LEFT_MOTOR:
            m_leftMotor.setPID(p, i, d);
            break;
        case RIGHT_MOTOR:
            m_rightMotor.setPID(p, i, d);
            break;
    }
}

float MotorControl::getMotorDrivenDistance(enum motor motor) {
    switch (motor) {
        case LEFT_MOTOR:
            return m_leftMotor.getDrivenDistance();
        case RIGHT_MOTOR:
            return m_rightMotor.getDrivenDistance();
    }
}

float MotorControl::getMotorSpeed(enum Board::IO::motor motor){
    switch (motor) {
        case LEFT_MOTOR:
            return m_leftMotor.getSpeed();
        case RIGHT_MOTOR:
            return m_rightMotor.getSpeed();
    }
}
