#include "ControlThread.hpp"
#include "DataStreamer.hpp"
#include "Logging.hpp"
#include "MotionBoard.hpp"
#include "Parameters.hpp"
#include "ch.hpp"
#include "cartesian/Pose_0_1.h"
#include "cartesian/Twist_0_1.h"
#include "cartesian/State_0_1.h"
#include "canard.h"
#include "CanProtocol.hpp"
#include "Quaternion.hpp"


enum ControlThreadEvents {
    BoardEvent      = 1 << 0,
};

ControlThread ControlThread::s_instance;

ControlThread* ControlThread::instance() {
    return &s_instance;
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
    Board::Com::CANBus::registerCanMsg(this,
                                       CanardTransferKindMessage,
                                       ROBOT_POSE_GOAL_ID,
                                       reg_udral_physics_kinematics_cartesian_Pose_0_1_EXTENT_BYTES_);
    Board::Com::CANBus::registerCanMsg(this,
                                      CanardTransferKindMessage,
                                      ROBOT_TWIST_GOAL_ID,
                                      reg_udral_physics_kinematics_cartesian_Twist_0_1_EXTENT_BYTES_);
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
                sendCurrentState();
                updateDataStreamer();

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

void ControlThread::sendCurrentState() {
    static CanardTransferID transfer_id = 0;

    struct ControlData controlData = control.getData();
    Quaternion q = Quaternion::Euler(0., 0., controlData.angle);
    const reg_udral_physics_kinematics_cartesian_State_0_1 state       = {
        .pose = {
            .position = {
                .value = {controlData.x * (1./1000.), controlData.y * (1./1000.), 0.},
            },
            .orientation = {
                .wxyz = {q.w, q.x, q.y, q.z},
            },
        },
        .twist = {
            .linear = {
                .meter_per_second = {controlData.linearSpeed, 0., 0.},
            },
            .angular = {
                .radian_per_second = {0., 0., controlData.angularSpeed},
            }
        },
    };

    size_t buf_size = reg_udral_physics_kinematics_cartesian_State_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_;
    uint8_t buffer[reg_udral_physics_kinematics_cartesian_State_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_];

    reg_udral_physics_kinematics_cartesian_State_0_1_serialize_(&state, buffer, &buf_size);


    const CanardTransferMetadata metadata = {
        .priority = CanardPriorityNominal,
        .transfer_kind = CanardTransferKindMessage,
        .port_id = ROBOT_CURRENT_STATE_ID,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id = transfer_id,
    };
    transfer_id++;
    Board::Com::CANBus::send(&metadata, buf_size,  buffer);

}

void ControlThread::processCanMsg(CanardRxTransfer * transfer) {
     switch(transfer->metadata.port_id) {
        case ROBOT_POSE_GOAL_ID:
            processPoseGoal(transfer);
            break;
        case ROBOT_TWIST_GOAL_ID:
            processTwistGoal(transfer);
            break;
        default:
            Logging::println("[Control Thread] CAN transfer dropped");
            break;
    }
}

void ControlThread::processPoseGoal(CanardRxTransfer * transfer) {
    reg_udral_physics_kinematics_cartesian_Pose_0_1 poseGoal;
    reg_udral_physics_kinematics_cartesian_Pose_0_1_deserialize_(&poseGoal,
                                                                 (uint8_t *)transfer->payload,
                                                                 &transfer->payload_size);
    float w, x, y, z;
    w = poseGoal.orientation.wxyz[0];
    x = poseGoal.orientation.wxyz[1];
    y = poseGoal.orientation.wxyz[2];
    z = poseGoal.orientation.wxyz[3];

    Quaternion q(w, x, y, z);
    float a, osef1, osef2, theta;
    a = 0.;
    q.ToAngleAxis(&a, &osef1, &osef2, &theta);
    Goal goal(x, y, theta, false);
    control.setCurrentGoal(goal);

}

void ControlThread::processTwistGoal(CanardRxTransfer * transfer) {
    reg_udral_physics_kinematics_cartesian_Twist_0_1 twistGoal;
    reg_udral_physics_kinematics_cartesian_Twist_0_1_deserialize_(&twistGoal,
                                                                 (uint8_t *)transfer->payload,
                                                                 &transfer->payload_size);
    float v = twistGoal.linear.meter_per_second[0] * 1000.; // linear speed on x axis;
    float w = twistGoal.angular.radian_per_second[2]; // angular speed on z axis;
    Logging::println("twist goal");
    Goal goal(v, w, Goal::GoalType::CIRCULAR);
    control.setCurrentGoal(goal);
}
