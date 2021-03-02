#include "LocalMath.hpp"
#include "Motor.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"
#include "DataStreamer.hpp"

using namespace Board::IO;

Motor::Motor(enum encoder encoder,
             enum motor motor,
             float wheelRadius) : m_wheelRadius(wheelRadius),
                                  m_encoder(encoder),
                                  m_motor(motor) {
    m_drivenDistance = 0.;
}

void Motor::updateControl() {
    updateMeasure();
    float command = m_speedPID.compute(m_speedSetpoint, m_speed);
    setMotorDutyCycle(m_motor, command);
    if (m_encoder == LEFT_ENCODER){
        DataStreamer::instance()->setEntry(leftPWMEnum, command);
    } else if (m_encoder == RIGHT_ENCODER){
        DataStreamer::instance()->setEntry(rightPWMEnum, command);
    }
}

void Motor::updateMeasure() {
    int16_t encoderCount = getEncoderCount(m_encoder);
    float drivenAngle = float(encoderCount) * (1. / ENCODER_TICK_PER_TURN) * GEAR_RATIO * 2. * M_PI;
    m_speed = drivenAngle * MOTOR_CONTROL_LOOP_FREQ * m_wheelRadius;
    m_drivenDistance += drivenAngle * m_wheelRadius;
}

void Motor::setPID(float p, float i, float d, float bias, float frequency) {
    m_speedPID.set(p, i, d, bias, frequency);
}

void Motor::setPID(float p, float i, float d) {
    m_speedPID.set(p, i, d);
}

void Motor::setSpeed(float speed) {
    m_speedSetpoint = speed;
}

void Motor::setWheelRadius(float wheelRadius){
    m_wheelRadius = wheelRadius;
}

float Motor::getSpeed() {
    return m_speed;
}

float Motor::getDrivenDistance() {
    float distance = m_drivenDistance;
    m_drivenDistance = 0.;
    return distance;
}

void Motor::reset() {
    m_speedPID.reset();
}