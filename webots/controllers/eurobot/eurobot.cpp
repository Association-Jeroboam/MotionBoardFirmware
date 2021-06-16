#include "ControlThread.hpp"
#include "Supervisor.hpp"
#include <iostream>
#include <memory>

extern std::shared_ptr<webots::Supervisor> supervisor;

int main() {
    auto controlThread = std::make_unique<ControlThread>();

    while (supervisor->step(8) != -1) {
        controlThread->main();
    }

    return 0;
}
