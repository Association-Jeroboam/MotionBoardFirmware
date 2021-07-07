#include "Motor.hpp"
#include "DataStreamer.hpp"
#include "LocalMath.hpp"
#include "Logging.hpp"
#include "MotionBoard.hpp"
#include "Parameters.hpp"
#include "Peripherals.hpp"

Motor::Motor(Peripherals::Encoder encoder,
             Peripherals::Motor   motor,
             float                wheelRadius) : m_wheelRadius(wheelRadius),
                                  m_encoder(encoder),
                                  m_motor(motor) {
    m_drivenDistance = 0.;
}

void Motor::updateControl() {
    updateMeasure();

    Board::IO::setMotorDutyCycle(m_motor, m_speedSetpoint);

    if (m_encoder == Peripherals::LEFT_ENCODER) {
        DataStreamer::instance()->setEntry(leftPWMEnum, m_speedSetpoint);
    } else if (m_encoder == Peripherals::RIGHT_ENCODER) {
        DataStreamer::instance()->setEntry(rightPWMEnum, m_speedSetpoint);
    }
}

void Motor::updateMeasure() {
    int16_t encoderCount = Board::IO::getEncoderCount(m_encoder);
    float   drivenAngle  = float(encoderCount) * (1. / ENCODER_TICK_PER_TURN) * GEAR_RATIO * 2. * M_PI;
    m_speed              = drivenAngle * MOTOR_CONTROL_LOOP_FREQ * m_wheelRadius;
    m_drivenDistance     = drivenAngle * m_wheelRadius;
}

void Motor::setSpeed(float speed) {
    m_speedSetpoint = speed;
}

void Motor::setWheelRadius(float wheelRadius) {
    m_wheelRadius = wheelRadius;
}

float Motor::getSpeed() {
    return m_speed;
}

float Motor::getDrivenDistance() {
    return m_drivenDistance;
}

void Motor::reset() {
    m_speedSetpoint = 0;
}
