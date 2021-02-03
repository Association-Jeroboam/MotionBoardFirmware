#include "Board.hpp"
#include "board.h"

__extension__ SerialConfig serialConfig{
        .speed = 115200,
        .cr1 = 0,
        .cr2 = USART_CR2_STOP1_BITS,
        .cr3 = 0
};
ioline_t Board::GPIO::getLedLine() {
    return LINE_ARD_D13;
}
