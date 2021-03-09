#include "ControlThread.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"

ControlThread* s_instance = nullptr;

ControlThread* ControlThread::instance() {
    if (s_instance == nullptr) {
        s_instance = new ControlThread();
    }

    return s_instance;
}

void ControlThread::main() {
    Logging::println("[Control] init");

    controlCounter++;

    if (controlCounter >= controlSteps) {
        controlCounter = 0;
        control.update();
    }
}

Control* ControlThread::getControl() {
    return &control;
}
