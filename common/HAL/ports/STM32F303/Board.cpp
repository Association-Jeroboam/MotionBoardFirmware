#include "ch.hpp"
#include "hal.h"
#include "hal_pal.h"
#include "hal_serial.h"
#include "hal_pwm.h"
#include "Board.hpp"
#include "board.h"
#include "BuildConf.hpp"

void Board::IO::initDrivers() {}

void Board::IO::deinitPWM(){}

void Board::IO::setMotorDutyCycle(enum motor motor, uint16_t duty_cycle){}

void Board::IO::toggleLED(){
    palToggleLine(LED_LINE);
}

void Board::Com::initDrivers() {}
