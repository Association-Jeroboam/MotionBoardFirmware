#include "Strategy/Strategy.hpp"

const Pos Strategy::positions[2][2] = {
    // Pos1
    {
        (Pos){ .x = 100, .y = 0 },
        (Pos){ .x = 100, .y = 1000 },
    },

    // Pos2
    {
        (Pos){ .x = 100, .y = 0 },
        (Pos){ .x = 100, .y = 1000 },
    },
};

Strategy Strategy::s_instance;

Strategy* Strategy::instance() {
    return &s_instance;
}
