#include <cmath>
#include "LocalMath.hpp"
#include "RobotPose.hpp"
#include "Parameters.hpp"

RobotPose::RobotPose(float x, float y, float angle) : m_x(x), m_y(y), m_angle(angle) {
    m_turns = 0;
}

void RobotPose::update(float dl, float dr) {
    float dDistance = (dr + dl) * 0.5;
    float dAngle = (dr - dl) / WHEEL_BASE;

    m_x += dDistance * cosf(m_angle + dAngle * 0.5);
    m_y += dDistance * sinf(m_angle + dAngle * 0.5);

    m_angle += dAngle;
    restrainAngle();

}

void RobotPose::setPose(float x, float y) {
    m_x = x;
    m_y = y;
}

void RobotPose::setPose(float x, float y, float angle) {
    m_angle = angle;
    m_turns = 0;
    restrainAngle();
    setPose(x, y);
}

float RobotPose::getX() {
    return m_x;
}

float RobotPose::getY() {
    return m_y;
}

float RobotPose::getAbsoluteAngle() {
    return m_turns * 2. * M_PI + m_angle;
}

float RobotPose::getModuloAngle() {
    return m_angle;
}

int32_t RobotPose::getTurns() {
    return m_turns;
}

void RobotPose::restrainAngle() {
    while (m_angle > M_PI || m_angle <= -M_PI) {
        if (m_angle > M_PI) {
            m_angle -= 2.0 * M_PI;
            m_turns++;
        } else if (m_angle <= -M_PI) {
            m_angle += 2.0 * M_PI;
            m_turns--;
        }
    }
}
