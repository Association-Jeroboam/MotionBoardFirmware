#pragma once

// namespace Strategy {
enum Event {
    StartMatch       = 1 << 0,
    CompassOk        = 1 << 1,
    StartFunnyAction = 1 << 2,
    EndMatch         = 1 << 3,
    EmergencyStop    = 1 << 4,
    MoveOk           = 1 << 5,
    CanMove          = 1 << 6
};
// }
