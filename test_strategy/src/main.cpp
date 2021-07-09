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


    fireEvent(StartMatch, "StartMatch");

    fireEvent(MoveOk, "MoveOk");
    robotPose->setPose(1700, -1700, M_PI);
    fireEvent(MoveOk, "MoveOk");


    robotPose->setPose(300, 230, 0.001);
    fireEvent(MoveOk, "MoveOk");


    robotPose->setPose(120, 230, 0.001);
    fireEvent(MoveOk, "MoveOk");

    robotPose->setPose(300, 230, 0.001);
    fireEvent(MoveOk, "MoveOk");
<<<<<<< HEAD
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
=======

>>>>>>> wip: angles
    fireEvent(StartFunnyAction, "StartFunnyAction");
    fireEvent(EndMatch, "EndMatch");
    fireEvent(EmergencyStop, "EmergencyStop");
}
