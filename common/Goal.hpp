#pragma once

#include <cstdint>


class Goal {
public:

    enum GoalType {
        DISTANCE,
        ANGLE,
        COORD,
        CIRCULAR,
        SPEED,
        PWM,
        NO_GOAL,
    };

    typedef struct coordData {
        float x;
        float y;
        float theta;
        bool forwardMovementOnly;
    } coordData_t;

    typedef struct circularData {
        float angularSpeed;
        float linearSpeed;
    } circularData_t;

    typedef struct speedData {
        float leftSpeed;
        float rightSpeed;
    } speedData_t;

    typedef struct pwmData {
        float leftPWM;
        float rightPWM;
        float time;
    } pwmData_t;

    typedef union goalData {
        coordData_t coordData;
        circularData_t circularData;
        speedData_t speedData;
        pwmData_t   pwmData;
    } goalData_t;

    Goal(float x, float y, float theta, bool forwardMovementOnly);

    Goal(float input0, float input1, enum GoalType type);

    Goal(float leftPWM, float rightPWM, float time);

    Goal();

    enum GoalType getType();

    coordData_t getCoordData();

    circularData_t getCircularData();

    speedData_t getSpeedData();

    pwmData_t getPWMData();

    uint32_t getID();

    void setReached(bool reached);

    bool isReached();

    void print();

private:
    enum GoalType m_type;
    goalData_t    m_data;
    uint32_t      m_ID;
    bool          m_reached;

    static uint32_t s_goalCount;
};
