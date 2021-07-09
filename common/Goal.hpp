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

    enum Direction {
        FORWARD,
        BACKWARD,
        ANY,
    };

    typedef struct distanceData {
        float distance;
    } distanceData_t;

    typedef struct angleData {
        float   angle;
        int32_t turns;
    } angleData_t;

    typedef struct coordData {
        float x;
        float y;
        enum Direction direction;
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
        angleData_t angleData;
        coordData_t coordData;
        circularData_t circularData;
        speedData_t speedData;
        pwmData_t   pwmData;
        distanceData_t  distanceData;
    } goalData_t;

    Goal(float distance);

    Goal(float angle, int32_t turns);

    Goal(float x, float y, enum Direction direction);

    Goal(float input0, float input1, enum GoalType type);

    Goal(float leftPWM, float rightPWM, float time);

    Goal();

    enum GoalType getType();

    distanceData_t  getDistanceData();

    angleData_t getAngleData();

    coordData_t getCoordData();

    circularData_t getCircularData();

    speedData_t getSpeedData();

    pwmData_t getPWMData();

    uint32_t getID();

    inline void setCoordDirection(enum Direction dir) { m_data.coordData.direction = dir;};

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
