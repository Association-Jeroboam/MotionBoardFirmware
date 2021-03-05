#include "ControlThread.hpp"
#include "DataStreamer.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"
#include <new>

ControlThread* s_instance = nullptr;

ControlThread* ControlThread::instance() {
    if (s_instance == nullptr) {
        s_instance = reinterpret_cast<ControlThread*>(chHeapAlloc(nullptr, sizeof(ControlThread)));
        new (s_instance) ControlThread();
    }
    return s_instance;
}

void ControlThread::main() {
    Logging::println("[Control] init");
    setName("Control");

    static uint16_t toggleCounter = 0;

    Board::Events::eventRegister(this, Board::Events::RUN_MOTOR_CONTROL);
    Board::Events::startControlLoop(MOTOR_CONTROL_LOOP_FREQ);

    while (!shouldTerminate()) {
        waitOneEvent(Board::Events::RUN_MOTOR_CONTROL);

        control.update();
        updateDataStreamer();

        toggleCounter++;
        if (toggleCounter % (MOTOR_CONTROL_LOOP_FREQ / LED_TOGGLE_FREQUENCY) == 0) {
            Board::IO::toggleLED();
            toggleCounter = 0;
        }
    }
}

void ControlThread::updateDataStreamer() {
    struct ControlData controlData = control.getData();

    DataStreamer::instance()->setEntry(angularSpeedEnum, controlData.angularSpeed);
    DataStreamer::instance()->setEntry(angularSpeedSetpointEnum, controlData.angularSpeedSetpoint);
    DataStreamer::instance()->setEntry(linearSpeedEnum, controlData.linearSpeed);
    DataStreamer::instance()->setEntry(linearSpeedSetpointEnum, controlData.linearSpeedSetpoint);
    DataStreamer::instance()->setEntry(angleEnum, controlData.angle);
    DataStreamer::instance()->setEntry(absoluteAngleEnum, controlData.absoluteAngle);
    DataStreamer::instance()->setEntry(angleSetpointEnum, controlData.angleSetpoint);
    DataStreamer::instance()->setEntry(distanceErrorEnum, controlData.distanceError);
    DataStreamer::instance()->setEntry(xEnum, controlData.x);
    DataStreamer::instance()->setEntry(yEnum, controlData.y);
}

Control* ControlThread::getControl() {
    return &control;
}
