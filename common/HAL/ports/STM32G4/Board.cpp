#include "ch.hpp"
#include "hal.h"
#include "hal_pal.h"
#include "hal_serial.h"
#include "hal_pwm.h"
#include "Board.hpp"
#include "board.h"
#include "BuildConf.hpp"

void Board::init(){
    Board::Com::initDrivers();
    Board::IO::initDrivers();
}

void Board::IO::initDrivers() {}

void Board::IO::deinitPWM(){}

void Board::IO::setMotorDutyCycle(enum motor motor, float duty_cycle){}

int16_t Board::IO::getEncoderCount(enum encoder encoder){}

void Board::IO::resetEncoder(enum encoder encoder){}

void Board::IO::toggleLED(){
    palToggleLine(LED_LINE);
}

void Board::Com::initDrivers() {}
