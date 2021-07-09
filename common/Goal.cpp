#include "Goal.hpp"
#include "Logging.hpp"

uint32_t Goal::s_goalCount = 0;

Goal::Goal(float distance){
    m_type = DISTANCE;
    m_data.distanceData.distance = distance;
    m_ID = s_goalCount;
    s_goalCount++;
    m_reached = false;
}


Goal::Goal(float angle, int32_t turns){
    m_type = ANGLE;
    m_data.angleData.angle = angle;
    m_data.angleData.turns = turns;
    m_ID = s_goalCount;
    s_goalCount++;
    m_reached = false;
}

Goal::Goal(float x, float y, enum Direction direction){
    m_type = COORD;
    m_data.coordData.x = x;
    m_data.coordData.y = y;
    m_data.coordData.direction = direction;
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
        case COORD:
            m_type = type;
            m_data.coordData.x = input0;
            m_data.coordData.y = input1;
            m_data.coordData.direction = ANY;
            m_ID = s_goalCount;
            s_goalCount++;
            break;
        case PWM:
            m_type = type;
            m_data.pwmData.leftPWM = input0;
            m_data.pwmData.rightPWM = input1;
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

Goal::distanceData_t Goal::getDistanceData(){
    return m_data.distanceData;
}

Goal::angleData_t Goal::getAngleData(){
    return m_data.angleData;
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
        case ANGLE:
            Logging::println("Angle Goal: ang %f turns %li", m_data.angleData.angle, m_data.angleData.turns);
            break;
        case COORD:
            Logging::println("Coord Goal: x %f y %f dir %u", m_data.coordData.x, m_data.coordData.y, m_data.coordData.direction);
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

    }
}
