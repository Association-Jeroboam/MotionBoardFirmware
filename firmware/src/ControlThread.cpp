#include "ControlThread.hpp"
#include "DataStreamer.hpp"
#include "Logging.hpp"
#include "MotionBoard.hpp"
#include "Parameters.hpp"
#include "ch.hpp"
#include "cartesian/Pose_0_1.h"
#include "canard.h"
#include "CanProtocol.hpp"


enum ControlThreadEvents {
    BoardEvent      = 1 << 0,
};

ControlThread ControlThread::s_instance;

ControlThread* ControlThread::instance() {
    return &s_instance;
}

void ControlThread::processPoseGoal(CanardRxTransfer * transfer) {
    reg_udral_physics_kinematics_cartesian_Pose_0_1 poseGoal;
    reg_udral_physics_kinematics_cartesian_Pose_0_1_deserialize_(&poseGoal,
                                                                 (uint8_t *)transfer->payload,
                                                                 &transfer->payload_size);
    Logging::println("Pose Goal");
    Logging::print("pos");
    for(uint8_t i = 0; i< 3; i++) {
        Logging::print(" %.2f", poseGoal.position.value.meter[i]);
    }
    Logging::println("\norientation");
    for(uint8_t i = 0; i< 4; i++) {
        Logging::print(" %.2f", poseGoal.orientation.wxyz[i]);
    }
    Logging::println("");
}

ControlThread::ControlThread() : BaseStaticThread<CONTROL_THREAD_WA>(),
                                 EventSource(),
                                 m_boardListener(),
                                 m_avoidanceListener(),
                                 moveOkFired(true) {
}

void ControlThread::main() {
    Logging::println("[ControlThread] init");
    setName("ControlThread");

    static uint16_t toggleCounter = 0;

    Board::Events::eventRegister(&m_boardListener, BoardEvent);
    Board::Com::CANBus::registerCanMsg(this, CanardTransferKindMessage, ROBOT_POSE_GOAL_ID, reg_udral_physics_kinematics_cartesian_Pose_0_1_EXTENT_BYTES_);
    Board::Events::startControlLoop(MOTOR_CONTROL_LOOP_FREQ);

    while (!shouldTerminate()) {
        eventmask_t event = waitOneEvent(BoardEvent);
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

void ControlThread::processCanMsg(CanardRxTransfer * transfer) {
     switch(transfer->metadata.port_id) {
        case ROBOT_POSE_GOAL_ID:
            processPoseGoal(transfer);
            break;
        default:
            break;
    }

}