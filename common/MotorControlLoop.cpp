#include "MotorControlLoop.hpp"
#include "Logging.hpp"
#include "Parameters.hpp"
#include "BuildConf.hpp"

constexpr uint16_t TOGGLE_FREQUENCY = 10; //[Hz]

using namespace Board::IO;

MotorControlLoop::MotorControlLoop(){
}
void MotorControlLoop::main(){
    Logging::println("[Motor Control Loop] init");
    setName("Motor Control Loop");

    Board::Events::eventRegister(this, Board::Events::RUN_MOTOR_CONTROL);
    Board::Events::startMotorControlLoop(MOTOR_CONTROL_LOOP_FREQ);

    static uint16_t toggleCounter = 0;

    while (!shouldTerminate()){
        waitOneEvent(Board::Events::RUN_MOTOR_CONTROL);

        toggleCounter++;
        if(toggleCounter%(MOTOR_CONTROL_LOOP_FREQ/TOGGLE_FREQUENCY) == 0){
            Board::IO::toggleLED();
            toggleCounter = 0;
        }

    }
    Logging::println("[Motor Control Loop] deinit");
}

void MotorControlLoop::motorSetSpeed(enum motor motor, float speed){
    switch(motor){
        case LEFT_MOTOR:
//            m_leftMotor.setSpeed(speed);
            break;
        case RIGHT_MOTOR:
//            m_rightMotor.setSpeed(speed);
            break;
    }
}

void MotorControlLoop::motorSetPID(enum motor motor, float p, float i, float d){
    switch(motor){
        case LEFT_MOTOR:
//            m_leftMotor.setPID(p, i, d);
            break;
        case RIGHT_MOTOR:
//            m_rightMotor.setPID(p, i, d);
            break;
    }
}
