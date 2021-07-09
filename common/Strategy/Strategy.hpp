#pragma once

#include "Control.hpp"
#include "Events.hpp"
#include "Goal.hpp"
#include "Switchers.hpp"
#include "Logging.hpp"
#include <cmath>

enum State {
    WAIT_FOR_MATCH,
    GO_TO_POS0,
    TURN_IN_POS0,
    GO_TO_POS1,
    LIGHTHOUSE,
    GO_TO_POS2,
    TURN_IN_POS2,
    GO_TO_POS3,
    TURN_IN_POS3,
    GO_TO_POS4,
    GO_TO_POS5,
    GO_TO_POS6,
    TURN_IN_POS5,
    WAIT_FUNNY_ACTION,
    END_MATCH
};

inline const char* eventToStr(Event event) {
    switch (event) {
        case StartMatch:
            return "StartMatch";
        case MoveOk:
            return "MoveOk";
        case StartFunnyAction:
            return "StartFunnyAction";
        case EndMatch:
            return "EndMatch";
        default:
            return "UnknownEvent";
    }
}

inline const char* stateToStr(State state) {
    switch (state) {
        case WAIT_FOR_MATCH:
            return "WaitForMatch";
        case GO_TO_POS0:
            return "GoToPos0";
        case TURN_IN_POS0:
            return "TurnInPos0";
        case GO_TO_POS1:
            return "GoToPos1";
        case TURN_IN_POS2:
            return "TurnInPos2";
        case GO_TO_POS2:
            return "GoToPos2";
        case TURN_IN_POS3:
            return "TurnInPos3";
        case GO_TO_POS3:
            return "GoToPos3";
        case GO_TO_POS4:
            return "GoToPos4";
        case WAIT_FUNNY_ACTION:
            return "WaitFunnyAction";
        case END_MATCH:
            return "EndMatch";
        case LIGHTHOUSE:
            return "Lighthouse";
        default:
            return "UnknownState";
    }
}

struct Pose {
    float x;
    float y;
    float theta;
};

class Strategy {
  private:

    static Strategy s_instance;

  public:
    Strategy();
    static Strategy* instance();
    Pose positions[11][2];
    static const float startX;
    static const float startY;
    static const float startAngle;


    void setControl(Control* control_) {
        control = control_;
    }

    // Match
    void setNewState(State newState) {
        RobotPose* robotPose = this->control->getRobotPose();

        currentState = newState;
        Logging::println("New state: %s", stateToStr(currentState));

        switch (currentState) {
            case GO_TO_POS0: {
                side = Switchers::getSide() ? 1 : 0;
                Logging::println("Loaded side %i", side);


                // Start right
                if (side == 1) {
                    robotPose->setPose(startX, SIMY(startY), -startAngle);
                    Logging::println("side right, init pos: %f %f %f", robotPose->getX(), robotPose->getY(), robotPose->getAbsoluteAngle());
                } else {
                    robotPose->setPose(startX, startY, startAngle);
                    Logging::println("side left, init pos: %f %f %f", robotPose->getX(), robotPose->getY(), robotPose->getAbsoluteAngle());
                }


                Pose targetPos = positions[0][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

                float dx = targetPos.x - robotPose->getX();
                float dy = targetPos.y - robotPose->getY();
                float distance = sqrtf(dx*dx + dy*dy);

                Goal goal(distance);
                this->control->setCurrentGoal(goal);
                Logging::println("Go to distance: %f", distance);

                break;
            }

            case TURN_IN_POS0: {
                float targetAngle = positions[1][side].theta;
                Logging::println("target angle: %f", targetAngle);

                            Goal goal(targetAngle, 0);
                this->control->setCurrentGoal(goal);

                break;
            }

            case GO_TO_POS1: {
                Pose targetPos = positions[2][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

                float dx = targetPos.x - robotPose->getX();
                float dy = targetPos.y - robotPose->getY();
                float distance = -sqrtf(dx*dx + dy*dy);

                // Marche arrière
                Goal goal(distance);
                this->control->setCurrentGoal(goal);
                Logging::println("Go to distance: %f", distance);

                break;
            }

            case LIGHTHOUSE: {
                Logging::println("TODO : bourrage");
                Goal goal(0., -150., Goal::CIRCULAR);
                this->control->setCurrentGoal(goal);
                break;
            }

            case GO_TO_POS2: {
                Pose targetPos = positions[3][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

                float dx = targetPos.x - robotPose->getX();
                float dy = targetPos.y - robotPose->getY();
                float distance = sqrtf(dx*dx + dy*dy);

                Goal goal(distance);
                this->control->setCurrentGoal(goal);
                Logging::println("Go to distance: %f", distance);

                break;
            }

            case TURN_IN_POS2: {
                float targetAngle = positions[4][side].theta;
                Logging::println("target angle: %f", targetAngle);

                            Goal goal(targetAngle, 0);
                this->control->setCurrentGoal(goal);

                break;
            }

            case GO_TO_POS3: {
                Pose targetPos = positions[5][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

                float dx = targetPos.x - robotPose->getX();
                float dy = targetPos.y - robotPose->getY();
                float distance = sqrtf(dx*dx + dy*dy);

                Goal goal(distance);
                this->control->setCurrentGoal(goal);
                Logging::println("Go to distance: %f", distance);

                break;
            }

            case TURN_IN_POS3: {
                float targetAngle = positions[6][side].theta;
                Logging::println("target angle: %f", targetAngle);

                            Goal goal(targetAngle, 0);
                this->control->setCurrentGoal(goal);

                break;
            }

            case GO_TO_POS4: {
                Pose targetPos = positions[7][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

                float dx = targetPos.x - robotPose->getX();
                float dy = targetPos.y - robotPose->getY();
                float distance = sqrtf(dx*dx + dy*dy);

                Goal goal(distance);
                this->control->setCurrentGoal(goal);
                Logging::println("Go to distance: %f", distance);
                break;
            }

            case GO_TO_POS5: {
                Pose targetPos = positions[8][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

                float dx = targetPos.x - robotPose->getX();
                float dy = targetPos.y - robotPose->getY();
                float distance = -sqrtf(dx*dx + dy*dy);

                Goal goal(distance);
                this->control->setCurrentGoal(goal);
                Logging::println("Go to distance: %f", distance);
                break;
            }

            case TURN_IN_POS5: {
                float targetAngle = positions[9][side].theta;
                Logging::println("target angle: %f", targetAngle);

                            Goal goal(targetAngle, 0);
                this->control->setCurrentGoal(goal);

                break;
            }

            case GO_TO_POS6: {
                Pose targetPos = positions[10][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

                float dx = targetPos.x - robotPose->getX();
                float dy = targetPos.y - robotPose->getY();
                float distance = sqrtf(dx*dx + dy*dy);

                Goal goal(distance);
                this->control->setCurrentGoal(goal);
                Logging::println("Go to distance: %f", distance);
                break;
            }


            case WAIT_FUNNY_ACTION: {
                Logging::println("Waiting for funny action...");

                break;
            }

            case END_MATCH: {
                Goal goal;
                this->control->setCurrentGoal(goal);
                Logging::println("End match : Goal set to NO_GOAL");

                break;
            }
        }
    }
    void dispatch(Event event) {
        Logging::println("Current state: %s", stateToStr(currentState));
        Logging::println("Event dispatched: %s\n", eventToStr(event));

        switch (currentState) {
            case WAIT_FOR_MATCH: {
                if (event == StartMatch) {
                    return setNewState(GO_TO_POS0);
                }
                
                break;
            }

            case GO_TO_POS0: {
                if (event == MoveOk) {
                   return setNewState(TURN_IN_POS0);
                }

                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                break;
            }

            case TURN_IN_POS0: {
                if (event == MoveOk) {
                   return setNewState(GO_TO_POS1);
                }

                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                break;
            }

            case GO_TO_POS1: {
                if (event == MoveOk) {
                   return setNewState(LIGHTHOUSE);
                }

                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                break;
            }


            case LIGHTHOUSE: {
                if (event == MoveOk) {
                   return setNewState(GO_TO_POS2);
                }

                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                break;
            }

            case GO_TO_POS2: {
                if (event == MoveOk) {
                   return setNewState(TURN_IN_POS2);
                }

                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                break;
            }

            case TURN_IN_POS2: {
                if (event == MoveOk) {
                   return setNewState(GO_TO_POS3);
                }

                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                break;
            }

            case GO_TO_POS3: {
                if (event == MoveOk) {
                   return setNewState(TURN_IN_POS3);
                }

                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                break;
            }

            case TURN_IN_POS3: {
                if (event == MoveOk) {
                   return setNewState(GO_TO_POS4);
                }

                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                break;
            }

            case GO_TO_POS4: { 
                if (event == MoveOk) {
                   return setNewState(GO_TO_POS5);
                }

                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                break;
            }

            case GO_TO_POS5: { 
                if (event == MoveOk) {
                   return setNewState(TURN_IN_POS5);
                }

                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                break;
            }

            case TURN_IN_POS5: { 
                if (event == MoveOk) {
                   return setNewState(GO_TO_POS6);
                }

                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                break;
            }

            case GO_TO_POS6: { 
                if (event == MoveOk) {
                   return setNewState(WAIT_FUNNY_ACTION);
                }

                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                break;
            }


            case WAIT_FUNNY_ACTION: {
                if (event == StartFunnyAction) {
                   // TODO : do funny action
                   Logging::println("Running funny action...");
                }

                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                break;
            }
        }
    }

    State currentState;
    Control* control;
    uint8_t side;
};
