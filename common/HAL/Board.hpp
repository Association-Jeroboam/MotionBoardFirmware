#pragma once

#include "ch.hpp"
#include "hal.h"
#include "hal_pal.h"
#include "hal_serial.h"

namespace Board {
    namespace IO {
        ioline_t getLedLine();
    }

    namespace Com {
        void initDrivers();
        SerialDriver * getLoggingDriver();
    }
}
