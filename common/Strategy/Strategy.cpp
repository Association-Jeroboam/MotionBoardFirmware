#include "Strategy/Strategy.hpp"

const Pos Strategy::positions[2][2] = {
    // Pos1
    {
        (Pos){ .x = 800, .y = 1200 },
        (Pos){ .x = 800, .y = SIMY(1200) },
    },

    // Pos2
    {
        (Pos){ .x = 1700, .y = 1065 },
        (Pos){ .x = 100, .y = SIMY(1065) },
    },
};

Strategy Strategy::s_instance;

Strategy* Strategy::instance() {
    return &s_instance;
}
