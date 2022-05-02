#include <stdio.h>

#include <string>

#include "Control.hpp"
#include "Strategy/Events.hpp"
#include "Strategy/Strategy.hpp"
#include "LocalMath.hpp"


Strategy* stateMachine;
Control   control;
RobotPose* robotPose;

void fireEvent(Event event, const char* eventName) {
    printf("\n[EVENT] %s\n", eventName);
    stateMachine->dispatch(event);
}

int main() {
    stateMachine = Strategy::instance();
    stateMachine->setControl(&control);
    robotPose = control.getRobotPose();

    robotPose->setPose(800, 200, M_PI/2);

    fireEvent(StartMatch, "StartMatch");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(StartFunnyAction, "StartFunnyAction");
    fireEvent(EndMatch, "EndMatch");
    fireEvent(EmergencyStop, "EmergencyStop");
}
