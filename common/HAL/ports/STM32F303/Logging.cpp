#include "Logging.hpp"
#include "Board.hpp"
#include <hal.h>

#include <chprintf.h>
#include <cstdarg>
#include "BuildConf.hpp"
#include "hal_serial.h"

__extension__ SerialConfig serialConfig {
        .speed = 115200,
        .cr1 = 0,
        .cr2 = USART_CR2_STOP1_BITS,
        .cr3 = 0
};

void Logging::init(){
    sdStart(&LOGGING_DRIVER, &serialConfig);
}

void Logging::print(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    chprintf(reinterpret_cast<BaseSequentialStream*>(&LOGGING_DRIVER), fmt, args);
    va_end(args);
}

void Logging::println(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    chprintf(reinterpret_cast<BaseSequentialStream*>(&LOGGING_DRIVER), fmt, args);
    chprintf(reinterpret_cast<BaseSequentialStream*>(&LOGGING_DRIVER), "\n");
    va_end(args);
}
