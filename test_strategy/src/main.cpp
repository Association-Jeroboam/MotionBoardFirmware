#include <stdio.h>

#include <string>

#include "HierarchicalStateMachine.hpp"
#include "StateMachine/Events.hpp"
#include "StateMachine/StateMachine.hpp"

using namespace eHSM;

StateMachine stateMachine;

void fireEvent(Event event, const char* eventName) {
    printf("\n[EVENT] %s\n", eventName);
    stateMachine.dispatch(event);
}

int main() {
    stateMachine.start();
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
