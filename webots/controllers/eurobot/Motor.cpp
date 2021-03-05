#include "Motor.hpp"
#include "LocalMath.hpp"
#include "Logging.hpp"
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
    // TODO
}

void Motor::updateMeasure() {
    // TODO
}

void Motor::setPID(float p, float i, float d, float bias, float frequency) {
    // TODO
}

void Motor::setPID(float p, float i, float d) {
    // TODO
}

void Motor::setSpeed(float speed) {
    // TODO
}

void Motor::setWheelRadius(float wheelRadius) {
    // TODO
}

float Motor::getSpeed() {
    // TODO
}

float Motor::getDrivenDistance() {
    // TODO
}

void Motor::reset() {
    // TODO
}
