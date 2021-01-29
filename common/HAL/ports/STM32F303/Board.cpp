#include "Board.hpp"
#include "board.h"

__extension__ SerialConfig serialConfig{
        .speed = 115200,
        .cr1 = 0,
        .cr2 = USART_CR2_STOP1_BITS,
        .cr3 = 0
};

ioline_t Board::GPIO::getLedLine() {
ioline_t Board::IO::getLedLine() {
    return LINE_ARD_D13;
}

void Board::Com::initDrivers() {
    sdStart(getLoggingDriver(), &serialConfig);
}

SerialDriver * Board::Com::getLoggingDriver() {
    return &SD2;
}
