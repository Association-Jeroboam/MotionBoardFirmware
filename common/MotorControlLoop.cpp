#include "MotorControlLoop.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"

constexpr uint16_t TOGGLE_FREQUENCY = 10; //[Hz]

using namespace Board::IO;

MotorControlLoop::MotorControlLoop() :
        m_leftMotor(LEFT_ENCODER, LEFT_MOTOR),
        m_rightMotor(RIGHT_ENCODER, RIGHT_MOTOR) {
    m_leftMotor.setPID(.2, 0.5, 0., 0., MOTOR_CONTROL_LOOP_FREQ);
    m_rightMotor.setPID(.2, 0.5, 0., 0., MOTOR_CONTROL_LOOP_FREQ);
}

void MotorControlLoop::main() {
    Logging::println("[Motor Control Loop] init");
    setName("Motor Control Loop");

    Board::Events::eventRegister(this, Board::Events::RUN_MOTOR_CONTROL);
    Board::Events::startMotorControlLoop(MOTOR_CONTROL_LOOP_FREQ);

    static uint16_t toggleCounter = 0;

    m_leftMotor.setSpeed(3.1415);
    m_rightMotor.setSpeed(3.1415);

    while (!shouldTerminate()) {
        waitOneEvent(Board::Events::RUN_MOTOR_CONTROL);
        m_leftMotor.updateControl();
        m_rightMotor.updateControl();

        toggleCounter++;
        if (toggleCounter % (MOTOR_CONTROL_LOOP_FREQ / TOGGLE_FREQUENCY) == 0) {
            toggleLED();
            toggleCounter = 0;
        }

    }
    Logging::println("[Motor Control Loop] deinit");
}

void MotorControlLoop::motorSetSpeed(enum motor motor, float speed) {
    switch (motor) {
        case LEFT_MOTOR:
            m_leftMotor.setSpeed(speed);
            break;
        case RIGHT_MOTOR:
            m_rightMotor.setSpeed(speed);
            break;
    }
}

void MotorControlLoop::motorSetPID(enum motor motor, float p, float i, float d) {
    switch (motor) {
        case LEFT_MOTOR:
            m_leftMotor.setPID(p, i, d);
            break;
        case RIGHT_MOTOR:
            m_rightMotor.setPID(p, i, d);
            break;
    }
}
