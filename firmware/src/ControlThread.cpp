#include "ControlThread.hpp"
#include "DataStreamer.hpp"
#include "Logging.hpp"
#include "MotionBoard.hpp"
#include "Parameters.hpp"
#include "Strategy/Events.hpp"
#include "Strategy/Strategy.hpp"
#include "ch.hpp"
#include "AvoidanceThread.hpp"
#include <new>

enum ControlThreadEvents {
    BoardEvent = 1 << 0,
    AvoidanceEvent  = 1 << 1,
};

ControlThread ControlThread::s_instance;

ControlThread* ControlThread::instance() {
    return &s_instance;
}

ControlThread::ControlThread() : BaseStaticThread<CONTROL_THREAD_WA>(), EventSource(), moveOkFired(true) {
}

void ControlThread::main() {
    Logging::println("[ControlThread] init");
    setName("ControlThread");

    static uint16_t toggleCounter = 0;

    Board::Events::eventRegister(&m_boardListener, BoardEvent);
    AvoidanceThread::instance()->registerMask(&m_avoidanceListener, AvoidanceEvent);
    Board::Events::startControlLoop(MOTOR_CONTROL_LOOP_FREQ);

    while (!shouldTerminate()) {
        eventmask_t event = waitOneEvent(AvoidanceEvent | BoardEvent);

        if( event && BoardEvent) {
            eventflags_t flags = m_boardListener.getAndClearFlags();
            if (flags & Board::Events::RUN_MOTOR_CONTROL) {

                control.update();

                bool goalReached = control.getCurrentGoal().isReached();

                if (goalReached) {
                    if (!moveOkFired) {
                        Logging::println("Goal reached");
                        //                    chThdSleep(TIME_MS2I(1));
                        this->broadcastFlags(GoalReached);
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

            if (flags & Board::Events::EMERGENCY_STOP) {
                Logging::println("[ControlThread] Emeregency Stop!");
                control.setEmergency(true);
            }

            if (flags & Board::Events::EMERGENCY_CLEARED) {
                Logging::println("[ControlThread] Emeregency Cleared");
                control.setEmergency(false);
            }
        }

        if(event & AvoidanceEvent) {
            eventflags_t flags = m_avoidanceListener.getAndClearFlags();
            if(flags & RobotDetected) {
                Logging::println("[ControlThread] Robot detected");
                control.setEmergency(true);
            }

            if(flags & WayCleared) {
                Logging::println("[ControlThread] Way cleared");
                control.setEmergency(false);
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
    DataStreamer::instance()->setEntry(xCurrentEnum, controlData.x);
    DataStreamer::instance()->setEntry(xSetpointEnum, controlData.xSetpoint);
    DataStreamer::instance()->setEntry(yCurrentEnum, controlData.y);
    DataStreamer::instance()->setEntry(ySetpointEnum, controlData.ySetpoint);
}

Control* ControlThread::getControl() {
    return &control;
}
