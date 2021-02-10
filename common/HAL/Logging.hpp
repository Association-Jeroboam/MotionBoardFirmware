#pragma once

#include <cstdint>

namespace Logging {

    void init();

    void print(const char *fmt, ...);

void println(const char* fmt, ...);

void setDisablePrint(bool disable);

void write(uint8_t *txBuffer, uint16_t len);

} // namespace Logging
