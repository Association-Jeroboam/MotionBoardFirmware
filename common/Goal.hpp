#pragma once

#include <cstdint>

class Goal {
  public:
    enum GoalType {
        POLAR,
        CIRCULAR,
        PWM,
        NO_GOAL,
    };

    enum Direction {
        FORWARD,
        BACKWARD,
        ANY,
    };

    typedef struct polarData {
        float   angle;
        int32_t turns;
        float   distance;
    } polarData_t;

    typedef struct circularData {
        float angularSpeed;
        float linearSpeed;
    } circularData_t;

    typedef struct pwmData {
        float leftPWM;
        float rightPWM;
        float time;
    } pwmData_t;

    typedef union goalData {
        polarData_t    polarData;
        circularData_t circularData;
        pwmData_t      pwmData;
    } goalData_t;

    Goal(float input0, float input1, enum GoalType type);

    Goal(float leftPWM, float rightPWM, float time);

    Goal();

    enum GoalType getType();

    polarData_t getPolarData();

    circularData_t getCircularData();

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
