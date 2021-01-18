#include <ch.h>
#include <hal.h>

#include <Logging.hpp>

int main() {
    halInit();
    chSysInit();

    Logging::init();
    Logging::println("Starting up");

    while (!chThdShouldTerminateX()) {
        chThdSleepMilliseconds(500);
    }

    Logging::println("Shutting down");
}
