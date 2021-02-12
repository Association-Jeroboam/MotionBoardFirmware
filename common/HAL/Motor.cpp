#include <cmath>
#include "Motor.hpp"
#include "Logging.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
using namespace Board::IO;

Motor::Motor(enum encoder encoder,
             enum motor motor) : m_encoder(encoder), m_motor(motor) {}

void Motor::updateControl() {
    updateSpeed();
    float error = m_speedSetpoint - m_speed;
    float command = m_speedPID.compute(error);
    setMotorDutyCycle(m_motor, command);
}

void Motor::updateSpeed() {
    int16_t encoderCount = getEncoderCount(m_encoder);
    float motor_speed = float(encoderCount) * (1. / ENCODER_TICK_PER_TURN) * MOTOR_CONTROL_LOOP_FREQ;
    float wheel_speed = motor_speed * GEAR_RATIO;
    m_speed = wheel_speed * 2. * M_PI;
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

float Motor::getSpeed() {
    return m_speed;
}
