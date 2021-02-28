#pragma once

#include <cstdint>

class RobotPose {
public:

    RobotPose(float x, float y, float angle);

    void update(float dl, float dr);

    void setPose(float x, float y);

    void setPose(float x, float y, float angle);

    float getX();

    float getY();

    float getAbsoluteAngle();

    float getModuloAngle();

    int32_t getTurns();

private:

    void restrainAngle();

    float m_x;
    float m_y;
    float m_angle;
    int32_t m_turns;
};
