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
    currentActionIndex = -1;
    actionNumber = 10;
    currentState = WAIT_FOR_MATCH;
    Action test[10][2] = {
        // Pos0
        {
            (Action){ actionType: GO_TO_XY, pos: (Pos){ x: 300, y: 230 }, backward: false },
            (Action){ actionType: GO_TO_XY, pos: (Pos){ x: 300, y: SIMY(230) }, backward: false },
        },

        // turn0
        {
            (Action){ actionType: TURN_TO, pos: (Pos){ x: 300, y: 230 }, backward: false, theta: 0.001 },
            (Action){ actionType: TURN_TO, pos: (Pos){ x: 300, y: SIMY(230) }, backward: false, theta: -0.001 },
        },

        // Pos1
        {
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 120, y: 230 }, backward: true },
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 120, y: SIMY(230) }, backward: true },
        },

        // Lighthouse
        {
            (Action) { actionType: DO_LIGHTHOUSE },
            (Action) { actionType: DO_LIGHTHOUSE },
        },

        // Pos2
        {
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 300, y: 230 }, backward: false },
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 300, y: SIMY(230) }, backward: false },
        },

        // Pos3
        {
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 1500, y: 1700 }, backward: false },
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 1500, y: SIMY(1700) }, backward: false },
        },

        // Pos4
        {
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 1700, y: 1700 }, backward: false },
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 1700, y: SIMY(1700) }, backward: false },
        },

        // Pos5
        {
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 1500, y: 1700 }, backward: true },
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 1500, y: SIMY(1700) }, backward: true },
        },

        // Pos6
        {
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 600, y: 1700 }, backward: false },
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 600, y: SIMY(1700) }, backward: false },
        },

        // Pos7
        {
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 600, y: 200 }, backward: false },
            (Action){ actionType: GO_TO_XY, pos: (Pos) { x: 600, y: SIMY(200) }, backward: false },
        },
    };

    std::memcpy(actions,test, sizeof(actions));
}

const float Strategy::startX = 655;
const float Strategy::startY = 230;
const float Strategy::startAngle = M_PI;

Strategy Strategy::s_instance;

Strategy* Strategy::instance() {
    return &s_instance;
}
