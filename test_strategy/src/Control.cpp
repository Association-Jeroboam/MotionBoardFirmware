#include "Control.hpp"
#include "Parameters.hpp"

Control::Control() : m_robotPose(INITIAL_X_POS, INITIAL_Y_POS, INITIAL_ANGLE) {
}

void Control::update() {
}

void Control::updateState() {
}

void Control::applyControl() {
}

void Control::setCurrentGoal(Goal goal) {
    goal.print();
}

Goal Control::getCurrentGoal() {
}

void Control::setAngleKp(float kp) {
}

void Control::setDistanceKp(float kp) {
}

void Control::setMotorPID(Peripherals::Motor motor, float p, float i, float d) {
}

void Control::reset() {
}

ControlData Control::getData() {
}

RobotPose* Control::getRobotPose() {
    return &m_robotPose;
}
