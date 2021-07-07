#include "Goal.hpp"
#include "Logging.hpp"

uint32_t Goal::s_goalCount = 0;

Goal::Goal(float input0, float input1, enum GoalType type) {
    switch (type) {
        case POLAR:
            m_type                    = type;
            m_data.polarData.angle    = input0;
            m_data.polarData.distance = input1;
            m_ID                      = s_goalCount;
            s_goalCount++;
            break;
        case CIRCULAR:
            m_type                           = type;
            m_data.circularData.angularSpeed = input0;
            m_data.circularData.linearSpeed  = input1;
            m_ID                             = s_goalCount;
            s_goalCount++;
            break;
        case PWM:
            m_type                  = type;
            m_data.pwmData.leftPWM  = input0;
            m_data.pwmData.rightPWM = input1;
            m_ID                    = s_goalCount;
            s_goalCount++;
            break;
        default:
            Logging::println("[Goal] Bad goal type. Creating NO_GOAL");
            Goal();
    }
    m_reached = false;
}

Goal::Goal(float leftPWM, float rightPWM, float time) {
    m_type                  = PWM;
    m_data.pwmData.leftPWM  = leftPWM;
    m_data.pwmData.rightPWM = rightPWM;
    m_data.pwmData.time     = time;
    m_ID                    = s_goalCount;
    s_goalCount++;
    m_reached = false;
}

Goal::Goal() {
    m_type = NO_GOAL;
    m_ID   = s_goalCount;
    s_goalCount++;
    m_reached = false;
}

enum Goal::GoalType Goal::getType() {
    return m_type;
}

Goal::polarData_t Goal::getPolarData() {
    return m_data.polarData;
}

Goal::circularData_t Goal::getCircularData() {
    return m_data.circularData;
}

Goal::pwmData_t Goal::getPWMData() {
    return m_data.pwmData;
}

uint32_t Goal::getID() {
    return m_ID;
}

void Goal::setReached(bool reached) {
    m_reached = reached;
}

bool Goal::isReached() {
    return m_reached;
}

void Goal::print() {
    switch (m_type) {
        case POLAR:
            Logging::println("Polar Goal: ang %.f turns %li distance %f", m_data.polarData.angle, m_data.polarData.turns, m_data.polarData.distance);
            break;
        case CIRCULAR:
            Logging::println("Circular Goal ang_spd %f lin_spd %f", m_data.circularData.angularSpeed, m_data.circularData.linearSpeed);
            ;
            break;
        case PWM:
            Logging::println("PWM goal, l_pwm %.2f r_pwm %.2f", m_data.pwmData.leftPWM, m_data.pwmData.rightPWM);
            break;
        case NO_GOAL:
            Logging::println("No Goal");
            break;
    }
}
