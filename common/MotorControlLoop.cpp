#include "MotorControlLoop.hpp"
#include "Logging.hpp"
#include "Board.hpp"
#include "Parameters.hpp"

constexpr uint16_t TOGGLE_FREQUENCY = 10; //[Hz]

MotorControlLoop::MotorControlLoop() {}

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
