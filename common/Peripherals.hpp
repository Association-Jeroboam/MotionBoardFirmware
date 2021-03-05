#pragma once
#include "stdint.h"

namespace Peripherals {

enum Encoder : uint8_t {
    LEFT_ENCODER  = 0,
    RIGHT_ENCODER = 1,
};

enum Motor : uint8_t {
    LEFT_MOTOR  = 1,
    RIGHT_MOTOR = 0,
};

} // namespace Peripherals