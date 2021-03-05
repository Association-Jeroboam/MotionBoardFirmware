#include <iostream>
#include <webots/Robot.hpp>

using namespace webots;

int main() {
    Robot* robot = new Robot();

    while (robot->step(8) != -1)
        std::cout << "Hello World!" << std::endl;

    delete robot;
    return 0;
}
