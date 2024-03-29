#include "ControlThread.hpp"
#include "DataStreamer.hpp"
#include "Logging.hpp"
#include "MotionBoard.hpp"
#include "Parameters.hpp"
#include "ch.hpp"
#include "State2D_1_0.h"
#include "canard.h"
#include "CanProtocol.hpp"
#include "AdaptativePIDConfig_0_1.h"
#include "PIDState_0_1.h"
#include "MotionConfig_0_1.h"
#include "SpeedCommand_0_1.h"
#include "OdometryTicks_0_1.h"

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
                                 moveOkFired(true),
                                 m_sendOdomTicks(false) {
}

void ControlThread::main() {
    Logging::println("[ControlThread] init");
    setName("ControlThread");

    Board::Events::eventRegister(&m_boardListener, BoardEvent);
    Board::Com::CANBus::registerCanMsg(this,
                                       CanardTransferKindMessage,
                                       ROBOT_POSE_GOAL_ID,
                                       jeroboam_datatypes_sensors_odometry_State2D_1_0_EXTENT_BYTES_);
    Board::Com::CANBus::registerCanMsg(this,
                                      CanardTransferKindMessage,
                                      ROBOT_TWIST_GOAL_ID,
                                       jeroboam_datatypes_sensors_odometry_Twist2D_1_0_EXTENT_BYTES_);
    Board::Com::CANBus::registerCanMsg(this,
                                       CanardTransferKindMessage,
                                       ROBOT_SET_CURRENT_POSE_ID,
                                       jeroboam_datatypes_sensors_odometry_Pose2D_1_0_EXTENT_BYTES_);
    Board::Com::CANBus::registerCanMsg(this,
                                       CanardTransferKindMessage,
                                       MOTION_SET_ADAPTATIVE_PID_ID,
                                       jeroboam_datatypes_actuators_motion_AdaptativePIDConfig_0_1_EXTENT_BYTES_);
    Board::Com::CANBus::registerCanMsg(this,
                                       CanardTransferKindMessage,
                                       MOTION_SET_MOTION_CONFIG_ID,
                                       jeroboam_datatypes_actuators_motion_MotionConfig_0_1_EXTENT_BYTES_);
    Board::Com::CANBus::registerCanMsg(this,
                                       CanardTransferKindMessage,
                                       ROBOT_GOAL_SPEEDS_WHEELS_ID,
                                       jeroboam_datatypes_actuators_motion_SpeedCommand_0_1_EXTENT_BYTES_);

    Board::Events::startControlLoop(MOTOR_CONTROL_LOOP_FREQ);

    uint8_t canPublishCounter = 0;

    while (!shouldTerminate()) {
        eventmask_t event = waitOneEvent(BoardEvent);
        if( event & BoardEvent) {

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
                canPublishCounter++;
                if( canPublishCounter >= CAN_PUBLISH_DIVIDER) {
                    if(m_sendOdomTicks) sendOdomTicks();
                    sendCurrentState();
                    sendPIDStates();
                    canPublishCounter = 0;
                }
                
                updateDataStreamer();

            }

            if (flags & Board::Events::EMERGENCY_STOP) {
                control.setEmergency(true);
            }

            if (flags & Board::Events::EMERGENCY_CLEARED) {
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

void ControlThread::sendOdomTicks() {
    static CanardTransferID transfer_id = 0;
    int32_t left, right;
    control.getMotorTicks(&left, &right);
    const jeroboam_datatypes_sensors_odometry_OdometryTicks_0_1 odom_ticks = {
        .left = left,
        .right = right,
    };
    size_t buf_size = jeroboam_datatypes_sensors_odometry_OdometryTicks_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_;
    uint8_t buffer[jeroboam_datatypes_sensors_odometry_OdometryTicks_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_];

    int8_t res = jeroboam_datatypes_sensors_odometry_OdometryTicks_0_1_serialize_(&odom_ticks, buffer, &buf_size);

    const CanardTransferMetadata metadata = {
        .priority = CanardPriorityImmediate,
        .transfer_kind = CanardTransferKindMessage,
        .port_id = MOTION_ODOM_TICKS_ID,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id = transfer_id,
    };
    if( res == NUNAVUT_SUCCESS) {
        transfer_id++;
        Board::Com::CANBus::send(&metadata, buf_size,  buffer);
    } else {
        Logging::println(" Could not serialize OdomTicks");
    }
}

void ControlThread::sendCurrentState() {
    static CanardTransferID transfer_id = 0;
    const float divideBy1000 = 1./1000.;

    struct ControlData controlData = control.getData();
    jeroboam_datatypes_sensors_odometry_State2D_1_0 state2D = {
        .pose = {
            .x = {.meter = controlData.x * divideBy1000},
            .y = {.meter = controlData.y * divideBy1000},
            .theta = {.radian = controlData.angle},
        },
        .twist = {
            .linear = {.meter_per_second = controlData.linearSpeed * divideBy1000},
            .angular = {.radian_per_second = controlData.angularSpeed},
        },
    };

    size_t buf_size = jeroboam_datatypes_sensors_odometry_State2D_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_;
    uint8_t buffer[jeroboam_datatypes_sensors_odometry_State2D_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_];

    int8_t res = jeroboam_datatypes_sensors_odometry_State2D_1_0_serialize_(&state2D, buffer, &buf_size);

    const CanardTransferMetadata metadata = {
        .priority = CanardPriorityImmediate,
        .transfer_kind = CanardTransferKindMessage,
        .port_id = ROBOT_CURRENT_STATE_ID,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id = transfer_id,
    };
    if( res == NUNAVUT_SUCCESS) {
        transfer_id++;
        Board::Com::CANBus::send(&metadata, buf_size,  buffer);
    }  else {
        Logging::println(" Could not serialize State");
    }
    

}

void ControlThread::sendPIDStates() {
    static CanardTransferID transfer_id = 0;

    SpeedControllerParameters params = control.getMotorControl()->getMotorControllerParameters(Peripherals::LEFT_MOTOR);
    jeroboam_datatypes_actuators_motion_PIDState_0_1 pidState;
    pidState.ID = CAN_PROTOCOL_LEFT_SPEED_PID_ID;
    pidState._error = params.speedError / 1000.;
    pidState.output = params.outputValue;
    pidState.setpoint = params.speedGoal / 1000.;

    size_t buf_size = jeroboam_datatypes_actuators_motion_PIDState_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_;
    uint8_t buffer[jeroboam_datatypes_actuators_motion_PIDState_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_];

    int8_t res = jeroboam_datatypes_actuators_motion_PIDState_0_1_serialize_(&pidState, buffer, &buf_size);

    CanardTransferMetadata metadata = {
        .priority = CanardPriorityNominal,
        .transfer_kind = CanardTransferKindMessage,
        .port_id = MOTION_PID_STATE_ID,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id = transfer_id,
    };
    if( res == NUNAVUT_SUCCESS) {
        transfer_id++;
        Board::Com::CANBus::send(&metadata, buf_size,  buffer);
    } else {
        Logging::println(" Could not serialize PID left");
    }

    params = control.getMotorControl()->getMotorControllerParameters(Peripherals::RIGHT_MOTOR);
    pidState.ID = CAN_PROTOCOL_RIGHT_SPEED_PID_ID;
    pidState._error = params.speedError / 1000.;
    pidState.output = params.outputValue;
    pidState.setpoint = params.speedGoal / 1000.;

    res = jeroboam_datatypes_actuators_motion_PIDState_0_1_serialize_(&pidState, buffer, &buf_size);

    metadata = {
        .priority = CanardPriorityNominal,
        .transfer_kind = CanardTransferKindMessage,
        .port_id = MOTION_PID_STATE_ID,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id = transfer_id,
    };
    if( res == NUNAVUT_SUCCESS) {
        transfer_id++;
        Board::Com::CANBus::send(&metadata, buf_size,  buffer);
    } else {
        Logging::println(" Could not serialize PID right");
    }

}

void ControlThread::processCanMsg(CanardRxTransfer * transfer) {
    switch(transfer->metadata.port_id) {
        case ROBOT_POSE_GOAL_ID: {
            float x, y, theta;
            processPoseMsg(transfer, &x, &y, &theta);
            Goal goal(x, y, theta, false);
            control.setCurrentGoal(goal);
            break;
        }
        case ROBOT_TWIST_GOAL_ID: {
            float linear, angular;
            processTwistMsg(transfer, &linear, &angular);
            Goal goal(angular, linear, Goal::GoalType::CIRCULAR);
            control.setCurrentGoal(goal);
            break;
        }
        case ROBOT_SET_CURRENT_POSE_ID: {
            Logging::println("[Control Thread] ROBOT_SET_CURRENT_POSE_ID");
            float x, y, theta;
            processPoseMsg(transfer, &x, &y, &theta);
            control.getRobotPose()->setPose(x, y, theta);
            break;
        }
        case MOTION_SET_ADAPTATIVE_PID_ID:
            processAdaptativPIDMsg(transfer);
            break;
        case MOTION_SET_MOTION_CONFIG_ID:
            processMotionConfigMsg(transfer);
            break;
        case ROBOT_GOAL_SPEEDS_WHEELS_ID:
            processSpeedCommandMsg(transfer);
            break;
        default:
            Logging::println("[Control Thread] CAN transfer dropped");
            break;
    }
}

void ControlThread::processPoseMsg(CanardRxTransfer * transfer, float* x, float* y, float* theta) {
    jeroboam_datatypes_sensors_odometry_Pose2D_1_0 pose2D;
    jeroboam_datatypes_sensors_odometry_Pose2D_1_0_deserialize_(&pose2D,
                                                                 (uint8_t *)transfer->payload,
                                                                 &transfer->payload_size);

    *x = pose2D.x.meter * 1000;
    *y = pose2D.y.meter * 1000;
    *theta = pose2D.theta.radian;
}

void ControlThread::processTwistMsg(CanardRxTransfer * transfer, float* linear, float* angular) {
    jeroboam_datatypes_sensors_odometry_Twist2D_1_0 twistGoal;
    jeroboam_datatypes_sensors_odometry_Twist2D_1_0_deserialize_(&twistGoal,
                                                                 (uint8_t *)transfer->payload,
                                                                 &transfer->payload_size);
    *linear = twistGoal.linear.meter_per_second * 1000.; // linear speed on x axis;
    *angular = twistGoal.angular.radian_per_second; // angular speed on z axis;
}

void ControlThread::processAdaptativPIDMsg(CanardRxTransfer * transfer) {
    jeroboam_datatypes_actuators_motion_AdaptativePIDConfig_0_1 adaptPID;
    jeroboam_datatypes_actuators_motion_AdaptativePIDConfig_0_1_deserialize_(&adaptPID,
                                                                                            (uint8_t *)transfer->payload,
                                                                                            &transfer->payload_size);
    switch (adaptPID.ID) {
        case CAN_PROTOCOL_LEFT_SPEED_PID_ID:
            for(uint8_t i = 0; i < NB_PI_SUBSET; i ++) {
                control.getMotorControl()->motorSetPID(Peripherals::LEFT_MOTOR, adaptPID.configs[i].pid[0], adaptPID.configs[i].pid[1], i);
                control.getMotorControl()->motorSetPIDThreshold(Peripherals::LEFT_MOTOR, adaptPID.thresholds[i] * 1000, i);
            }


            break;
        case CAN_PROTOCOL_RIGHT_SPEED_PID_ID:
            for(uint8_t i = 0; i < NB_PI_SUBSET; i ++) {
                control.getMotorControl()->motorSetPID(Peripherals::RIGHT_MOTOR, adaptPID.configs[i].pid[0], adaptPID.configs[i].pid[1], i);
                control.getMotorControl()->motorSetPIDThreshold(Peripherals::RIGHT_MOTOR, adaptPID.thresholds[i] * 1000, i);
            }
            break;
        default:
            Logging::println("[Control] Unknown motor id");
    }
}

void ControlThread::processMotionConfigMsg(CanardRxTransfer * transfer) {
    jeroboam_datatypes_actuators_motion_MotionConfig_0_1 motionConf;
    int8_t res = jeroboam_datatypes_actuators_motion_MotionConfig_0_1_deserialize_(&motionConf,
                                                                      (uint8_t *)transfer->payload,
                                                                      &transfer->payload_size);
    if(res == NUNAVUT_SUCCESS) {
        control.getRobotPose()->setWheelBase(motionConf.wheel_base.meter * 1000);
        control.getMotorControl()->setWheelRadius(Peripherals::LEFT_MOTOR, motionConf.left_wheel_radius.meter * 1000);
        control.getMotorControl()->setWheelRadius(Peripherals::RIGHT_MOTOR, motionConf.right_wheel_radius.meter * 1000);
    } else {
        Logging::println("[Control] Failed deserialize speed command");
    }
}

void ControlThread::processSpeedCommandMsg(CanardRxTransfer * transfer) {
    jeroboam_datatypes_actuators_motion_SpeedCommand_0_1 speedCommand;
    int8_t res = jeroboam_datatypes_actuators_motion_SpeedCommand_0_1_deserialize_(&speedCommand,
                                                                                   (uint8_t *)transfer->payload,
                                                                                   &transfer->payload_size);
    if(res == NUNAVUT_SUCCESS) {
        Goal goal(speedCommand.left.meter_per_second * 1000, speedCommand.right.meter_per_second * 1000, Goal::SPEED);
        control.setCurrentGoal(goal);
    } else {
        Logging::println("[Control] Failed deserialize speed command");
    }

}

void ControlThread::setPublishTicksState(bool publish) {
    m_sendOdomTicks = publish;
    }
