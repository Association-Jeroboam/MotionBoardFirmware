#include "Control.hpp"
#include "Parameters.hpp"
#include "Supervisor.hpp"
#include <iostream>
#include <memory>

using namespace webots;

int main() {
    auto               control        = std::make_unique<Control>();
    const unsigned int controlSteps   = MOTOR_CONTROL_LOOP_DT * 1000 / 8;
    unsigned int       controlCounter = 0;

    while (supervisor->step(8) != -1) {
        controlCounter++;
        if (controlCounter >= controlSteps) {
            controlCounter = 0;
            control->update();
        }
    }

    return 0;
}
