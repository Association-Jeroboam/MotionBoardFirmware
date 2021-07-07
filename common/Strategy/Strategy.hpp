#pragma once

#include "Control.hpp"
#include "Events.hpp"
#include "Goal.hpp"
#include "Switchers.hpp"
#include "Logging.hpp"
#include <cmath>

enum State {
    WAIT_FOR_MATCH,
    TURN_TO_POS0,
    GO_TO_POS0,
    TURN_TO_POS1,
    GO_TO_POS1,
};

inline const char* eventToStr(Event event) {
    switch (event) {
        case StartMatch:
            return "StartMatch";
        case MoveOk:
            return "MoveOk";
        default:
            return "UnknownEvent";
    }
}

inline const char* stateToStr(State state) {
    switch (state) {
        case WAIT_FOR_MATCH:
            return "WaitForMatch";
        case TURN_TO_POS0:
            return "TurnToPos0";
        case GO_TO_POS0:
            return "GoToPos0";
        case TURN_TO_POS1:
            return "TurnToPos1";
        case GO_TO_POS1:
            return "GoToPos1";
        default:
            return "UnknownState";
    }
}

struct Pos {
    float x;
    float y;
};

class Strategy {
  private:
    Strategy() {
        currentState = WAIT_FOR_MATCH;
    };

    static Strategy s_instance;

  public:
    static Strategy* instance();
    static const Pos positions[2][2];


    void setControl(Control* control_) {
        control = control_;
    }

    void dispatch(Event event) {
        Logging::println("Current state: %s", stateToStr(currentState));
        Logging::println("Event dispatched: %s\n", eventToStr(event));

        switch (currentState) {
            case WAIT_FOR_MATCH: {
                if (event != StartMatch)
                    return;

                currentState = TURN_TO_POS0;
                Logging::println("New state: %s", stateToStr(currentState));

                side = Switchers::getSide() ? 1 : 0;
                Logging::println("Loaded side %i", side);

                RobotPose* robotPose = this->control->getRobotPose();

                // Start right
                if (side == 1) {
                    robotPose->setPose(robotPose->getX(), SIMY(robotPose->getY()), -robotPose->getAbsoluteAngle());
                }

                Pos targetPos = positions[0][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

                float targetAngle = atan2(targetPos.y - robotPose->getY(), targetPos.x - robotPose->getX());

                Goal goal(targetAngle, Goal::ANGLE);
                this->control->setCurrentGoal(goal);

                // TODO goal angle
                Logging::println("Turn to: %f", targetAngle);
                break;
            }

            case TURN_TO_POS0: {
                if (event != MoveOk)
                    return;

                currentState = GO_TO_POS0;
                Logging::println("New state: %s", stateToStr(currentState));

                Pos targetPos = positions[0][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);
                Goal goal(targetPos.x, targetPos.y, Goal::COORD);
                
                this->control->setCurrentGoal(goal);
                Logging::println("Go to: %f %f", targetPos.x, targetPos.y);

                break;
            }

            case GO_TO_POS0: {
                if (event != MoveOk)
                    return;

                currentState = TURN_TO_POS1;
                Logging::println("New state: %s", stateToStr(currentState));

                Pos targetPos = positions[1][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);
                RobotPose* robotPose = this->control->getRobotPose();

                float targetAngle = atan2(targetPos.y - robotPose->getY(), targetPos.x - robotPose->getX());

                Goal goal(targetAngle, Goal::ANGLE);
                this->control->setCurrentGoal(goal);

                // TODO goal angle
                Logging::println("Turn to: %f", targetAngle);
                break;
            }

            case TURN_TO_POS1: {
                if (event != MoveOk)
                    return;

                currentState = GO_TO_POS1;
                Logging::println("New state: %s", stateToStr(currentState));

                Pos targetPos = positions[1][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);
                Goal goal(targetPos.x, targetPos.y, Goal::COORD);
                
                this->control->setCurrentGoal(goal);
                Logging::println("Go to: %f %f", targetPos.x, targetPos.y);

            }
        }
    }

    State currentState;
    Control* control;
    uint8_t side;
};
