#include "Logging.hpp"

#include <cstdarg>
#include <iostream>

void Logging::print(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::printf(fmt, args);
    va_end(args);
}

void Logging::println(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::printf(fmt, args);
    std::printf("\n");
    va_end(args);
}
