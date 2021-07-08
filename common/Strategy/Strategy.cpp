#include "Strategy/Strategy.hpp"
#include "LocalMath.hpp"

// Homologation
// const Pos Strategy::positions[2][2] = {
//     // Pos1
//     {
//         (Pose){ .x = 800, .y = 1200 },
//         (Pose){ .x = 800, .y = SIMY(1200) },
//     },

//     // Pos2
//     {
//         (Pose){ .x = 1700, .y = 1200 },
//         (Pose){ .x = 1700, .y = SIMY(1200) },
//     },
// };
// const float Strategy::startX = 800;
// const float Strategy::startY = 200;
// const float Strategy::startAngle = M_PI/2;

// Match
const Pose Strategy::positions[3][2] = {
    // Pos0
    {
        (Pose){ .x = 250, .y = 175 },
        (Pose){ .x = 250, .y = SIMY(175) },
    },

    // Pos1
    {
        (Pose){ .x = 250, .y = 150, .theta = 0.001 },
        (Pose){ .x = 250, .y = SIMY(175), .theta = -0.001 },
    },

    // Pos2
    {
        (Pose){ .x = 70, .y = 175 },
        (Pose){ .x = 70, .y = SIMY(175) },
    },
};
const float Strategy::startX = 655;
const float Strategy::startY = 175;
const float Strategy::startAngle = M_PI;

Strategy Strategy::s_instance;

Strategy* Strategy::instance() {
    return &s_instance;
}
