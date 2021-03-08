#include "Motor.hpp"
#include "LocalMath.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"
#include "Peripherals.hpp"
#include "Supervisor.hpp"
#include <webots/Motor.hpp>

webots::Motor* getWebotMotor(Peripherals::Motor motor) {
    std::string motorName;

    if (motor == Peripherals::LEFT_MOTOR) {
        motorName = "left wheel motor";
    } else {
        motorName = "right wheel motor";
    }

    webots::Motor* webotsMotor = supervisor->getMotor(motorName);

    return webotsMotor;
}

Motor::Motor(Peripherals::Encoder encoder,
             Peripherals::Motor   motor,
             float                wheelRadius) : m_wheelRadius(wheelRadius),
                                  m_encoder(encoder),
                                  m_motor(motor) {
    m_drivenDistance = 0.;

    auto webotsMotor = getWebotMotor(m_motor);
    webotsMotor->setPosition(INFINITY);
    webotsMotor->setVelocity(0);
}

void Motor::updateControl() {
    updateMeasure();
}

void Motor::updateMeasure() {
    float wheelRadius;

    if (m_motor == Peripherals::LEFT_MOTOR) {
        wheelRadius = WHEEL_LEFT_RADIUS;
    } else {
        wheelRadius = WHEEL_RIGHT_RADIUS;
    }

    auto motor = getWebotMotor(m_motor);
    m_speed    = motor->getVelocity() * wheelRadius;
}

void Motor::setPID(float p, float i, float d, float bias, float frequency) {
    (void)p;
    (void)i;
    (void)d;
    (void)bias;
    (void)frequency;
}

void Motor::setPID(float p, float i, float d) {
    (void)p;
    (void)i;
    (void)d;
}

void Motor::setSpeed(float speed) {
    float wheelRadius;

    if (m_motor == Peripherals::LEFT_MOTOR) {
        wheelRadius = WHEEL_LEFT_RADIUS;
    } else {
        wheelRadius = WHEEL_RIGHT_RADIUS;
    }

    auto motor = getWebotMotor(m_motor);
    motor->setVelocity(speed / wheelRadius);
}

void Motor::setWheelRadius(float wheelRadius) {
    m_wheelRadius = wheelRadius;
}

float Motor::getSpeed() {
    return m_speed;
}

float Motor::getDrivenDistance() {
    return m_speed * MOTOR_CONTROL_LOOP_DT;
}

void Motor::reset() {
}
