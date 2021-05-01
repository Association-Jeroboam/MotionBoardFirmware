#include "Motor.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"
#include "Peripherals.hpp"

Motor::Motor(Peripherals::Encoder encoder,
             Peripherals::Motor   motor,
             float                wheelRadius) : m_wheelRadius(wheelRadius),
                                  m_encoder(encoder),
                                  m_motor(motor) {
}

void Motor::updateControl() {
}

void Motor::updateMeasure() {
}

void Motor::setPID(float p, float i, float d, float bias, float frequency) {
}

void Motor::setPID(float p, float i, float d) {
}

void Motor::setSpeed(float speed) {
}

void Motor::setWheelRadius(float wheelRadius) {
}

float Motor::getSpeed() {
}

float Motor::getDrivenDistance() {
}

void Motor::reset() {
}
