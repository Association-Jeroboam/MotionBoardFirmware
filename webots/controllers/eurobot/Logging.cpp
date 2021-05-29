#include "Logging.hpp"

//#include <libioP.h>
//#include <stdarg.h>
#include <cstdarg>
//#include <iostream>
#include <stdio.h>

void Logging::print(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}

void Logging::println(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    printf("\n");
    va_end(args);
}
