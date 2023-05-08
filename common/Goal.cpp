#include "Goal.hpp"
#include "Logging.hpp"
#include <algorithm>

uint32_t Goal::s_goalCount = 0;


Goal::Goal(float x, float y, float theta, bool forwardMovementOnly){
    m_type = COORD;
    m_data.coordData.x = x;
    m_data.coordData.y = y;
    m_data.coordData.theta = theta;
    m_data.coordData.forwardMovementOnly = forwardMovementOnly;
    m_ID = s_goalCount;
    s_goalCount++;
    m_reached = false;
}

Goal::Goal(float input0, float input1, enum GoalType type){
    switch (type) {
        case CIRCULAR:
            m_type = type;
            m_data.circularData.angularSpeed = input0;
            m_data.circularData.linearSpeed  = input1;
            m_ID = s_goalCount;
            s_goalCount++;
            break;
        case SPEED:
            m_type = type;
            m_data.speedData.leftSpeed  = input0;
            m_data.speedData.rightSpeed = input1;
            m_ID = s_goalCount;
            s_goalCount++;
            break;
        case PWM:
            m_type = type;
            // cap between -1 and 1
            m_data.pwmData.leftPWM  = std::max(std::min(input0, 1.f), -1.f );
            m_data.pwmData.rightPWM = std::max(std::min(input1, 1.f), -1.f );
            m_ID = s_goalCount;
            s_goalCount++;
            break;
        default:
            Logging::println("[Goal] Bad goal type. Creating NO_GOAL");
            Goal();
    }
    m_reached = false;
}

Goal::Goal(float leftPWM, float rightPWM, float time){
    m_type = PWM;
    m_data.pwmData.leftPWM = leftPWM;
    m_data.pwmData.rightPWM = rightPWM;
    m_data.pwmData.time = time;
    m_ID = s_goalCount;
    s_goalCount++;
    m_reached = false;
}

Goal::Goal(){
    m_type = NO_GOAL;
    m_ID = s_goalCount;
    s_goalCount++;
    m_reached = false;
}

enum Goal::GoalType Goal::getType(){
    return m_type;
}

Goal::coordData_t Goal::getCoordData(){
    return m_data.coordData;
}

Goal::circularData_t Goal::getCircularData() {
    return m_data.circularData;
}

Goal::speedData_t Goal::getSpeedData(){
    return m_data.speedData;
}

Goal::pwmData_t Goal::getPWMData(){
    return m_data.pwmData;
}

uint32_t Goal::getID(){
    return m_ID;
}

void Goal::setReached(bool reached){
    m_reached = reached;
}

bool Goal::isReached(){
    return m_reached;
}

void Goal::print() {
    switch (m_type) {
        case COORD:
            Logging::println("Coord Goal: x %f y %f theta %f forwardOnly %u", m_data.coordData.x, m_data.coordData.y, m_data.coordData.theta, m_data.coordData.forwardMovementOnly);
            break;
        case CIRCULAR:
            Logging::println("Circular Goal ang_spd %f lin_spd %f", m_data.circularData.angularSpeed, m_data.circularData.linearSpeed);;
            break;
        case SPEED:
            Logging::println("Speed goal l_spd %f r_spd %f", m_data.speedData.leftSpeed, m_data.speedData.rightSpeed);
            break;
        case PWM:
            Logging::println("PWM goal, l_pwm %.2f r_pwm %.2f", m_data.pwmData.leftPWM, m_data.pwmData.rightPWM);
            break;
        case NO_GOAL:
            Logging::println("No Goal");
            break;
        case DISTANCE:
            Logging::println("Distance goal");
            break;
        case ANGLE:
            Logging::println("Angle goal");
            break;

    }
}
