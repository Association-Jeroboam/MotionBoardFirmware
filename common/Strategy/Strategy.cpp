#include "Strategy/Strategy.hpp"

Strategy* inst = nullptr;

Strategy* Strategy::instance() {
    if (inst == nullptr) {
        inst = new Strategy();
    }

    return inst;
}
