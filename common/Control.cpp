#include <new>
#include <cmath>
#include "LocalMath.hpp"
#include "Control.hpp"
#include "Parameters.hpp"
#include "Logging.hpp"
#include "Board.hpp"
#include "DataStreamer.hpp"


using namespace Board::IO;

Control * s_instance = nullptr;

Control * Control::instance() {
    if (s_instance == nullptr) {
        s_instance = reinterpret_cast<Control *>(chHeapAlloc(nullptr, sizeof(Control)));
        new(s_instance) Control();
    }
    return s_instance;
}

Control::Control() : m_robotPose(INITIAL_X_POS, INITIAL_Y_POS, INITIAL_ANGLE){
    m_distancePID.set(DISTANCE_KP, 0., 0., 0., MOTOR_CONTROL_LOOP_FREQ);
    m_anglePID.set(ANGLE_KP, 0., 0., 0., MOTOR_CONTROL_LOOP_FREQ);
    m_distancePID.setMaxOutput( MAX_WHEEL_SPEED);
    m_anglePID.setMaxOutput(MAX_ANGULAR_SPEED);
    m_linearSpeed          = 0.;
    m_linearSpeedSetpoint  = 0.;
    m_angularSpeed         = 0.;
    m_angularSpeedSetpoint = 0.;
    m_angleSetpoint        = INITIAL_ANGLE;
    m_lastAngleSetpoint    = INITIAL_ANGLE;
    m_distanceError        = 0.;
    m_computeDirection = true;
}

void Control::main() {
    Logging::println("[Control] init");
    setName("Control");

    static uint16_t toggleCounter = 0;

    Board::Events::eventRegister(this, Board::Events::RUN_MOTOR_CONTROL);
    Board::Events::startControlLoop(MOTOR_CONTROL_LOOP_FREQ);

    while (!shouldTerminate()){
        waitOneEvent(Board::Events::RUN_MOTOR_CONTROL);

        updateState();

        applyControl();

        updateDataStreamer();

        toggleCounter++;
        if (toggleCounter % (MOTOR_CONTROL_LOOP_FREQ / LED_TOGGLE_FREQUENCY) == 0) {
            Board::IO::toggleLED();
            toggleCounter = 0;
        }

    }

}

void Control::updateState() {
    float dl = m_motorControl.getMotorDrivenDistance(LEFT_MOTOR);
    float dr = m_motorControl.getMotorDrivenDistance(RIGHT_MOTOR);

    float left_speed = m_motorControl.getMotorSpeed(LEFT_MOTOR);
    float right_speed = m_motorControl.getMotorSpeed(RIGHT_MOTOR);

    m_robotPose.update(dl, dr);
    m_linearSpeed  = (right_speed + left_speed) * 0.5;
    m_angularSpeed = (right_speed - left_speed) / WHEEL_BASE;
}

void Control::applyControl() {
    float leftSpeedSetpoint;
    float rightSpeedSetpoint;

    switch (m_currentGoal.getType()) {
        case Goal::ANGLE: {
            m_angleSetpoint = m_currentGoal.getAngleData().angle + m_currentGoal.getAngleData().turns * 2 * M_PI;
            m_angularSpeedSetpoint = m_anglePID.compute(m_angleSetpoint, m_robotPose.getAbsoluteAngle());
            m_linearSpeedSetpoint = 0.;
            break;
        }
        case Goal::COORD: {
            float xError = m_currentGoal.getCoordData().x - m_robotPose.getX();
            float yError = m_currentGoal.getCoordData().y - m_robotPose.getY();
            float angleToGoal = atan2(yError, xError);

            if(m_computeDirection) {
                switch (m_currentGoal.getCoordData().direction) {
                    case Goal::FORWARD:
                        m_forwardDrive = true;
                        break;
                    case Goal::BACKWARD:
                        m_forwardDrive = false;
                        break;
                    case Goal::ANY:
                        float angleError = angleToGoal - m_robotPose.getModuloAngle();
                        angleError = moduloTwoPI(angleError);
                        if (-M_PI * 0.5 < angleError && angleError < M_PI * 0.5) {
                            m_forwardDrive = true;
                        } else {
                            m_forwardDrive = false;
                        }
                        break;
                }
                m_computeDirection = false;
            }

            if(m_forwardDrive){
                m_angleSetpoint = angleToGoal;
                m_distanceError = sqrtf( xError * xError + yError * yError);
            } else{

                if(angleToGoal > M_PI * 0.5) {
                    m_angleSetpoint = -M_PI;
                } else {
                    m_angleSetpoint = M_PI;
                }
                m_angleSetpoint += angleToGoal;
                m_distanceError = -sqrtf( xError * xError + yError * yError);
            }
            m_angleSetpoint = unwrap(m_lastAngleSetpoint, m_angleSetpoint);

            m_angularSpeedSetpoint = m_anglePID.compute(m_angleSetpoint, m_robotPose.getAbsoluteAngle());
            m_lastAngleSetpoint = m_angleSetpoint;
            //prevents motor saturation
            float maxLinearSpeed = MAX_WHEEL_SPEED - fabsf(m_angularSpeedSetpoint) * WHEEL_BASE * 0.5;

            m_distancePID.setMaxOutput(maxLinearSpeed);
            m_linearSpeedSetpoint = m_distancePID.compute(m_distanceError, 0.);
            if (m_distanceError < COMPUTE_DIRECTION_THRESHOLD){
                m_computeDirection = true;
            }
            break;
        }
        case Goal::CIRCULAR: {

            m_angularSpeedSetpoint = m_currentGoal.getCircularData().angularSpeed;
            m_linearSpeedSetpoint  = m_currentGoal.getCircularData().linearSpeed;
            break;
        }
        case Goal::SPEED: {
            Logging::println("speed");
            leftSpeedSetpoint = m_currentGoal.getSpeedData().leftSpeed;
            rightSpeedSetpoint = m_currentGoal.getSpeedData().rightSpeed;
            goto set_speeds;
        }
        case Goal::PWM: {
            //TODO
            break;
        }
        case Goal::NO_GOAL:{
            m_linearSpeed          = 0.;
            m_linearSpeedSetpoint  = 0.;
            m_angularSpeed         = 0.;
            m_angularSpeedSetpoint = 0.;
            m_angleSetpoint        = 0.;
            m_distanceError        = 0.;
            m_motorControl.resetMotor(LEFT_MOTOR);
            m_motorControl.resetMotor(RIGHT_MOTOR);
            break;
        }
    }

    leftSpeedSetpoint  = m_linearSpeedSetpoint - m_angularSpeedSetpoint * WHEEL_BASE * 0.5;
    rightSpeedSetpoint = m_linearSpeedSetpoint + m_angularSpeedSetpoint * WHEEL_BASE * 0.5;
set_speeds:
    m_motorControl.motorSetSpeed(LEFT_MOTOR, leftSpeedSetpoint);
    m_motorControl.motorSetSpeed(RIGHT_MOTOR, rightSpeedSetpoint);
    m_motorControl.update();
}

void Control::updateDataStreamer() {
    DataStreamer::instance()->setEntry(angularSpeedEnum, m_angularSpeed);
    DataStreamer::instance()->setEntry(angularSpeedSetpointEnum, m_angularSpeedSetpoint);
    DataStreamer::instance()->setEntry(linearSpeedEnum, m_linearSpeed);
    DataStreamer::instance()->setEntry(linearSpeedSetpointEnum, m_linearSpeedSetpoint);
    DataStreamer::instance()->setEntry(angleEnum, m_robotPose.getModuloAngle());
    DataStreamer::instance()->setEntry(absoluteAngleEnum, m_robotPose.getAbsoluteAngle());
    DataStreamer::instance()->setEntry(angleSetpointEnum, m_angleSetpoint);
    DataStreamer::instance()->setEntry(distanceErrorEnum, m_distanceError);
    DataStreamer::instance()->setEntry(xEnum, m_robotPose.getX());
    DataStreamer::instance()->setEntry(yEnum, m_robotPose.getY());

}

void Control::setCurrentGoal(Goal goal){
    m_currentGoal = goal;
    m_computeDirection = true;
    m_currentGoal.print();
}

Goal Control::getCurrentGoal() {
    return m_currentGoal;
}

void Control::setAngleKp(float kp){
    m_anglePID.set(kp, 0., 0.);
    m_motorControl.resetMotor(LEFT_MOTOR);
    m_motorControl.resetMotor(RIGHT_MOTOR);
}

void Control::setDistanceKp(float kp){
    m_distancePID.set(kp, 0., 0.);
    m_motorControl.resetMotor(LEFT_MOTOR);
    m_motorControl.resetMotor(RIGHT_MOTOR);
}

void Control::setMotorPID(Board::IO::motor motor,float p, float i, float d){
    m_motorControl.motorSetPID(motor, p, i, d);
}

void Control::reset(){
    m_robotPose.setPose(INITIAL_X_POS, INITIAL_Y_POS, INITIAL_ANGLE);
    m_anglePID.reset();
    m_distancePID.reset();
    m_motorControl.resetMotor(LEFT_MOTOR);
    m_motorControl.resetMotor(RIGHT_MOTOR);
    m_linearSpeed          = 0.;
    m_linearSpeedSetpoint  = 0.;
    m_angularSpeed         = 0.;
    m_angularSpeedSetpoint = 0.;
    m_angleSetpoint        = 0.;
    m_distanceError        = 0.;
    m_currentGoal = Goal();
}
