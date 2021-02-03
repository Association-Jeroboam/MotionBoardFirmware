#pragma once

#include "ch.hpp"
#include "hal.h"
#include "hal_pal.h"
#include "hal_serial.h"

namespace Board {
    namespace GPIO {
        ioline_t getLedLine();
    }
}
