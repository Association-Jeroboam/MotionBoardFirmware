#include "Logging.hpp"
#include "Board.hpp"
#include <hal.h>

#include <chprintf.h>
#include <cstdarg>

void Logging::print(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    chprintf(reinterpret_cast<BaseSequentialStream*>(Board::Com::getLoggingDriver()), fmt, args);
    va_end(args);
}

void Logging::println(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    chprintf(reinterpret_cast<BaseSequentialStream*>(Board::Com::getLoggingDriver()), fmt, args);
    chprintf(reinterpret_cast<BaseSequentialStream*>(Board::Com::getLoggingDriver()), "\n");
    va_end(args);
}
