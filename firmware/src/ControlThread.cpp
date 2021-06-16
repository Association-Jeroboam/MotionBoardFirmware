#include "ControlThread.hpp"
#include "Board.hpp"
#include "DataStreamer.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"
#include "Strategy/Events.hpp"
#include "Strategy/Strategy.hpp"
#include "ch.hpp"
#include <new>

static ControlThread* s_instance = nullptr;

ControlThread* ControlThread::instance() {
    if (s_instance == nullptr) {
        s_instance = reinterpret_cast<ControlThread*>(chHeapAlloc(nullptr, sizeof(ControlThread)));
        new (s_instance) ControlThread();
    }
    return s_instance;
}

ControlThread::ControlThread() : BaseStaticThread<CONTROL_THREAD_WA>(), EventListener(), EventSource(), moveOkFired(true) {
}

void ControlThread::main() {
    Logging::println("[ControlThread] init");
    setName("ControlThread");

    static uint16_t toggleCounter = 0;

    Board::Events::eventRegister(this, RunMotorControl);
    Board::Events::startControlLoop(MOTOR_CONTROL_LOOP_FREQ);

    while (!shouldTerminate()) {
        waitOneEvent(RunMotorControl);
        eventflags_t flags = getAndClearFlags();
        if(flags & Board::Events::RUN_MOTOR_CONTROL) {

            control.update();

            bool goalReached = control.getCurrentGoal().isReached();

            if (goalReached) {
                if (!moveOkFired) {
                    Logging::println("Goal reached");
                    //                    chThdSleep(TIME_MS2I(1));
                    this->broadcastFlagsI(GoalReached);
                    Logging::println("Goal reached returned");
                    moveOkFired = true;
                }
            } else {
                if (moveOkFired) {
                    Logging::println("new goal");
                    moveOkFired = false;
                }
            }

            updateDataStreamer();

            toggleCounter++;
            if (toggleCounter % (MOTOR_CONTROL_LOOP_FREQ / LED_TOGGLE_FREQUENCY) == 0) {
                Board::IO::toggleLED();
                toggleCounter = 0;
            }
        }
    }

    Logging::println("[ControlThread] end");
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
