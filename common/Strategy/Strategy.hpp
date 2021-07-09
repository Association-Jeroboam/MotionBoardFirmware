#pragma once

#include "Control.hpp"
#include "Events.hpp"
#include "Goal.hpp"
#include "Switchers.hpp"
#include "Logging.hpp"
#include <cmath>

enum State {
    WAIT_FOR_MATCH,
    ANGLE_GOAL,
    DISTANCE_GOAL,
    LIGHTHOUSE,
    WAIT_FUNNY_ACTION,
    END_MATCH
};

enum ActionType {
    GO_TO_XY,
    TURN_TO,
    DO_LIGHTHOUSE
};

struct Pos {
    float x;
    float y;
};

struct Action {
    ActionType actionType;
    Pos pos;
    bool backward;
    float theta;
};

inline const char* eventToStr(Event event) {
    switch (event) {
        case StartMatch:
            return "StartMatchEvent";
        case MoveOk:
            return "MoveOkEvent";
        case StartFunnyAction:
            return "StartFunnyActionEvent";
        case EndMatch:
            return "EndMatchEvent";
        default:
            return "UnknownEvent";
    }
}

inline const char* stateToStr(State state) {
    switch (state) {
        case WAIT_FOR_MATCH:
            return "WaitForMatchState";
        case ANGLE_GOAL:
            return "AngleGoalState";
        case DISTANCE_GOAL:
            return "DistanceGoalState";
        case WAIT_FUNNY_ACTION:
            return "WaitFunnyActionState";
        case END_MATCH:
            return "EndMatchState";
        case LIGHTHOUSE:
            return "LighthouseState";
        default:
            return "UnknownState";
    }
}

inline const char* actionToStr(Action action) {
    switch (action.actionType) {
        case GO_TO_XY:
            return "GoToXYAction";
        case ANGLE_GOAL:
            return "AngleGoalAction";
        case DISTANCE_GOAL:
            return "DistanceGoalAction";
        case LIGHTHOUSE:
            return "LighthouseAction";
        default:
            return "UnknownState";
    }
}


class Strategy {
  private:

    static Strategy s_instance;

  public:
    Strategy();
    static Strategy* instance();
    Action actions[13][2];
    static const float startX;
    static const float startY;
    static const float startAngle;


    void setControl(Control* control_) {
        control = control_;
    }


    void go_to_pos(Pos targetPos, bool backward) {
        RobotPose* robotPose = this->control->getRobotPose();        
        Logging::println("Target pos x: %f y: %f", targetPos.x, targetPos.y);
        float dx = targetPos.x - robotPose->getX();
        float dy = targetPos.y - robotPose->getY();

        float distance = sqrtf(dx*dx + dy*dy);

        if (backward) {
            distance = -distance;
        }

        Goal goal(distance);
        this->control->setCurrentGoal(goal);
        Logging::println("Go to distance: %f", distance);
    }

    void turn(float targetAngle) {
        Logging::println("target angle: %f", targetAngle);
        Goal goal(targetAngle, 0);
        this->control->setCurrentGoal(goal);
    }


    // Match
    void setNewState(State newState) {
        RobotPose* robotPose = this->control->getRobotPose();

        currentState = newState;
        Logging::println("New state: %s", stateToStr(currentState));

        switch (currentState) {
            case State::ANGLE_GOAL: {
                Action currentAction = actions[currentActionIndex][side];

                float currentAngle = robotPose->getAbsoluteAngle();
                float targetAngle;
                
                if (currentAction.actionType == ActionType::TURN_TO) {
                    targetAngle = currentAction.theta;
                } else if (currentAction.actionType == ActionType::GO_TO_XY) {
                    float dy = currentAction.pos.y - robotPose->getY();
                    float dx = currentAction.pos.x - robotPose->getX();
                    targetAngle = atan2f(dy, dx);

                    if (currentAction.backward) {
                        targetAngle = constrainAngle(targetAngle + M_PI);
                    }
                } else {
                    Logging::println("Impossible 2");
                    return;
                }

                Logging::println("current: %f target: %f", currentAngle, targetAngle);

                // 5 deg
                if (angleDistance(currentAngle, targetAngle)  >= 0.09) {
                    return turn(targetAngle);
                } 

                Logging::println("Angle already reached");
                setNewState(State::DISTANCE_GOAL);;
                break;
            }

            case State::DISTANCE_GOAL: {
                Pos targetPos = actions[currentActionIndex][side].pos;
                bool backward = actions[currentActionIndex][side].backward;
                go_to_pos(targetPos, backward);
                break;
            }

            case State::LIGHTHOUSE: {
                Goal goal(0., -150., Goal::CIRCULAR);
                this->control->setCurrentGoal(goal);
                break;
            }

            case State::WAIT_FUNNY_ACTION: {
                Logging::println("Waiting for funny action...");

                break;
            }

            case State::END_MATCH: {
                Goal goal;
                this->control->setCurrentGoal(goal);
                Logging::println("End match : Goal set to NO_GOAL");

                break;
            }
        }
    }

    void doNextAction() {
        currentActionIndex++;
        Logging::println("Do action %i", currentActionIndex);

        if (currentActionIndex >= actionNumber) {
            Logging::println("No more action, end match");
            return setNewState(END_MATCH);
        }

        Action currentAction = actions[currentActionIndex][side];
        Logging::println(actionToStr(currentAction));

        switch (currentAction.actionType) {
            case GO_TO_XY: {
                setNewState(ANGLE_GOAL);
                break;
            }
            case TURN_TO: {
                setNewState(ANGLE_GOAL);
                break;
            }
            case DO_LIGHTHOUSE: {
                setNewState(LIGHTHOUSE);
                break;
            }
        }

    }

    void dispatch(Event event) {
        Logging::println("Current state: %s", stateToStr(currentState));
        Logging::println("Event dispatched: %s\n----------------------------------------------------------------------------\n", eventToStr(event));

        RobotPose* robotPose = this->control->getRobotPose();

        switch (currentState) {
            case State::WAIT_FOR_MATCH: {
                if (event == StartMatch) {
                    side = Switchers::getSide() ? 1 : 0;

                    // Start right
                    if (side == 1) {
                        robotPose->setPose(startX, SIMY(startY), -startAngle);
                        Logging::println("side right, init pos: %f %f %f", robotPose->getX(), robotPose->getY(), robotPose->getAbsoluteAngle());
                    } else {
                        robotPose->setPose(startX, startY, startAngle);
                        Logging::println("side left, init pos: %f %f %f", robotPose->getX(), robotPose->getY(), robotPose->getAbsoluteAngle());
                    }

                    return doNextAction();
                }
                
                break;
            }

            case State::ANGLE_GOAL: {
                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                Action currentAction = actions[currentActionIndex][side];
                if (event == MoveOk) {
                    // XY action, after rotation go to target
                    if (currentAction.actionType == GO_TO_XY) {
                        return setNewState(DISTANCE_GOAL);
                    }

                    // turn action, after rotation do next action
                    if (currentAction.actionType == TURN_TO) {
                        return doNextAction();
                    }

                    Logging::println("Impossible");
                }

                break;
            }

            case State::DISTANCE_GOAL: {
                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                if (event == MoveOk) {
                    return doNextAction();
                }

                break;
            }

            case LIGHTHOUSE: {
                if (event == EndMatch) {
                    return setNewState(END_MATCH);
                }

                if (event == MoveOk) {
                   return doNextAction();
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
    int16_t currentActionIndex;
    size_t actionNumber;
};
