#include "Point.hpp"
#include "RobotPose.hpp"
#include <cmath>

Point Point::robotToMap(Point point, RobotPose pose){
    float robot_x = pose.getX();
    float robot_y = pose.getY();
    float robot_angle = pose.getModuloAngle();
    float sin = sinf(robot_angle);
    float cos = cosf(robot_angle);

    return Point(point.x() * cos - point.y() * sin - (robot_x * cos - robot_y * sin),
                 point.x() * sinf(-robot_angle) + point.y() * cosf(-robot_angle) - (robot_x * sin + robot_y * cos) );
}

Point Point::mapToRobot(Point point, RobotPose pose){
    float robot_x = pose.getX();
    float robot_y = pose.getY();
    float robot_angle = pose.getModuloAngle();
    float sin = sinf(robot_angle);
    float cos = cosf(robot_angle);

    return Point(point.x() * cos - point.y() * sin + robot_x,
                 point.x() * sin + point.y() * cos + robot_y);
}

Point Point::polarToCartesian(float angle, float distance){
    return Point(distance * cosf(angle), distance * sinf(angle));
}
