#include "MotorControl.hpp"
#include "Parameters.hpp"
#include "Peripherals.hpp"

MotorControl::MotorControl() : m_leftMotor(Peripherals::LEFT_ENCODER, Peripherals::LEFT_MOTOR, WHEEL_LEFT_RADIUS),
                               m_rightMotor(Peripherals::RIGHT_ENCODER, Peripherals::RIGHT_MOTOR, WHEEL_RIGHT_RADIUS) {
}

void MotorControl::update() {
}

void MotorControl::motorSetSpeed(Peripherals::Motor motor, float speed) {
    switch (motor) {
    }
}

void MotorControl::motorSetPID(Peripherals::Motor motor, float p, float i, float d) {
    switch (motor) {
    }
}

float MotorControl::getMotorDrivenDistance(Peripherals::Motor motor) {
}

float MotorControl::getMotorSpeed(Peripherals::Motor motor) {
}

void MotorControl::resetMotor(Peripherals::Motor motor) {
}
