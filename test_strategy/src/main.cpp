#include <stdio.h>

#include <string>

#include "Control.hpp"
#include "Strategy/Events.hpp"
#include "Strategy/Strategy.hpp"


Strategy* stateMachine;
Control   control;

void fireEvent(Event event, const char* eventName) {
    printf("\n[EVENT] %s\n", eventName);
    stateMachine->dispatch(event);
}

int main() {
    stateMachine = Strategy::instance();
    stateMachine->setControl(&control);

    fireEvent(StartMatch, "StartMatch");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(CanMove, "CanMove");
    fireEvent(MoveOk, "MoveOk");
    fireEvent(CanMove, "CanMove");
    fireEvent(CompassOk, "CompassOk");
    fireEvent(StartFunnyAction, "StartFunnyAction");
    fireEvent(EndMatch, "EndMatch");
    fireEvent(EmergencyStop, "EmergencyStop");
}
