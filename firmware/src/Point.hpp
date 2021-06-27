#pragma once

class RobotPose;

class Point {
  private:
    float m_x;
    float m_y;
  public:
    explicit Point(float x = 0., float y = 0.):m_x(x), m_y(y){};
    inline float x() const{return m_x;};
    inline float y() const{return m_y;};
    static Point robotToMap(Point point, RobotPose pose);
    static Point mapToRobot(Point point, RobotPose pose);
    static Point polarToCartesian(float angle, float distance);

};
