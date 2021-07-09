#include "Strategy/Strategy.hpp"
#include "LocalMath.hpp"
#include <cstring>

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

Strategy::Strategy() {
    currentState = WAIT_FOR_MATCH;
    Pose test[][2] = {
        // Pos0
        {
            (Pose){ .x = 300, .y = 230 },
            (Pose){ .x = 300, .y = SIMY(230) },
        },

        // turn0
        {
            (Pose){ .x = 300, .y = 230, .theta = 0.001 },
            (Pose){ .x = 300, .y = SIMY(230), .theta = -0.001 },
        },

        // Pos1
        {
            (Pose){ .x = 120, .y = 230 },
            (Pose){ .x = 120, .y = SIMY(230) },
        },

        // Pos2
        {
            (Pose){ .x = 300, .y = 230 },
            (Pose){ .x = 300, .y = SIMY(230) },
        },

        // turn2
        {
            (Pose){ .x = 300, .y = 230, .theta = atan2f(1700-230, 1500-300) },
            (Pose){ .x = 300, .y = SIMY(230), .theta = -atan2f(1700-230, 1500-300) },
        },


        // Pos3
        {
            (Pose){ .x = 1500, .y = 1700 },
            (Pose){ .x = 1500, .y = SIMY(1700) },
        },


        // turn3
        {
            (Pose){ .x = 1500, .y = 1700, .theta = 0.0001 },
            (Pose){ .x = 1500, .y = SIMY(1700), .theta = -0.0001 },
        },

        // Pos4
        {
            (Pose){ .x = 1700, .y = 1700 },
            (Pose){ .x = 1700, .y = SIMY(1700) },

        },
        //pos5
        {
            (Pose){ .x = 1500, .y = 1700 },
            (Pose){ .x = 1500, .y = SIMY(1700) },

        },
        //turn5
        {
            (Pose){ .x = 1500, .y = 1700 , .theta = M_PI},
            (Pose){ .x = 1500, .y = SIMY(1700), .theta = -M_PI },

        },



        // Pos6
        {
            (Pose){ .x = 600, .y = 1700 },
            (Pose){ .x = 600, .y = SIMY(1700) },

        },

        // Turn6
        {
            (Pose){ .x = 600, .y = 1700, .theta = -M_PI/2 +0.0001},
            (Pose){ .x = 600, .y = SIMY(1700), .theta = M_PI/2 - 0.001},
        },


        // Pos7
        {
            (Pose){ .x = 600, .y = 200 },
            (Pose){ .x = 600, .y = SIMY(250) },

        },

/*        // Turn7
        {
            (Pose){ .x = 600, .y = 700, .theta = M_PI/4 },
            (Pose){ .x = 600, .y = SIMY(700), .theta = -M_PI/4 },
        },

        // Pos8
        {
            (Pose){ .x = 600, .y = 200 },
            (Pose){ .x = 600, .y = SIMY(200) },

        }*/

    };

    std::memcpy(positions,test, sizeof(positions));
}

const float Strategy::startX = 655;
const float Strategy::startY = 230;
const float Strategy::startAngle = M_PI;

Strategy Strategy::s_instance;

Strategy* Strategy::instance() {
    return &s_instance;
}
