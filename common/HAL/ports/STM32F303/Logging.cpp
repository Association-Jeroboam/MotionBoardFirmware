#include "Logging.hpp"

#include <hal.h>

#include <chprintf.h>
#include <cstdarg>

#define LOGGING_SERIAL_DRIVER SD2

void Logging::init() {
    sdStart(&LOGGING_SERIAL_DRIVER, nullptr);
}

void Logging::print(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    chprintf(reinterpret_cast<BaseSequentialStream*>(&LOGGING_SERIAL_DRIVER), fmt, args);
    va_end(args);
}

void Logging::println(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    chprintf(reinterpret_cast<BaseSequentialStream*>(&LOGGING_SERIAL_DRIVER), fmt, args);
    chprintf(reinterpret_cast<BaseSequentialStream*>(&LOGGING_SERIAL_DRIVER), "\n");
    va_end(args);
}
