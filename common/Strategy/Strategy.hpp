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
    Strategy() {
        currentState = WAIT_FOR_MATCH;
    };

    static Strategy s_instance;

  public:
    static Strategy* instance();
    static const Pose positions[8][2];
    static const float startX;
    static const float startY;
    static const float startAngle;


    void setControl(Control* control_) {
        control = control_;
    }

    // Homologation
    // void setNewState(State newState) {
    //     currentState = newState;
    //     Logging::println("New state: %s", stateToStr(currentState));

    //     switch (currentState) {
    //         case GO_TO_POS0: {
    //             side = Switchers::getSide() ? 1 : 0;
    //             Logging::println("Loaded side %i", side);

    //             RobotPose* robotPose = this->control->getRobotPose();

    //             // Start right
    //             if (side == 1) {
    //                 robotPose->setPose(startX, SIMY(startY), -startAngle);
    //                 Logging::println("side right, init pos: %f %f %f", robotPose->getX(), robotPose->getY(), robotPose->getAbsoluteAngle());
    //             } else {
    //                 robotPose->setPose(startX, startY, startAngle);
    //                 Logging::println("side left, init pos: %f %f %f", robotPose->getX(), robotPose->getY(), robotPose->getAbsoluteAngle());
    //             }


    //             Pose targetPos = positions[0][side];
    //             Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

    //             Goal goal(targetPos.x, targetPos.y, Goal::COORD);
    //             this->control->setCurrentGoal(goal);
    //             Logging::println("Go to: %f %f", targetPos.x, targetPos.y);

    //             break;
    //         }

    //         case TURN_TO_POS1: {
    //             Pose targetPos = positions[1][side];
    //             Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

    //             RobotPose* robotPose = this->control->getRobotPose();

    //             float targetAngle = atan2(targetPos.y - robotPose->getY(), targetPos.x - robotPose->getX());
    //             Goal goal(targetAngle, Goal::ANGLE);
    //             this->control->setCurrentGoal(goal);
    //             Logging::println("Turn to: %f", targetAngle);

    //             break;
    //         }

    //         case GO_TO_POS1: {
    //             Pose targetPos = positions[1][side];
    //             Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

    //             Goal goal(targetPos.x, targetPos.y, Goal::COORD);
    //             this->control->setCurrentGoal(goal);
    //             Logging::println("Go to: %f %f", targetPos.x, targetPos.y);

    //             break;
    //         }

    //         case WAIT_FUNNY_ACTION: {
    //             Logging::println("Waiting for funny action...");

    //             break;
    //         }

    //         case END_MATCH: {
    //             Goal goal;
    //             this->control->setCurrentGoal(goal);
    //             Logging::println("End match : Goal set to NO_GOAL");

    //             break;
    //         }
    //     }
    // }
    // void dispatch(Event event) {
    //     Logging::println("Current state: %s", stateToStr(currentState));
    //     Logging::println("Event dispatched: %s\n", eventToStr(event));

    //     switch (currentState) {
    //         case WAIT_FOR_MATCH: {
    //             if (event == StartMatch) {
    //                 return setNewState(GO_TO_POS0);
    //             }
                
    //             break;
    //         }

    //         case GO_TO_POS0: {
    //             if (event == MoveOk) {
    //                return setNewState(TURN_TO_POS1);
    //             }

    //             if (event == EndMatch) {
    //                 return setNewState(END_MATCH);
    //             }

    //             break;
    //         }

    //         case TURN_TO_POS1: {
    //             if (event == MoveOk) {
    //                return setNewState(GO_TO_POS1);
    //             }

    //             if (event == EndMatch) {
    //                 return setNewState(END_MATCH);
    //             }

    //             break;
    //         }

    //         case GO_TO_POS1: {
    //             if (event == MoveOk) {
    //                return setNewState(WAIT_FUNNY_ACTION);
    //             }

    //             if (event == EndMatch) {
    //                 return setNewState(END_MATCH);
    //             }

    //             break;
    //         }

    //         case WAIT_FUNNY_ACTION: {
    //             if (event == StartFunnyAction) {
    //                // TODO : do funny action
    //                Logging::println("Running funny action...");
    //             }

    //             if (event == EndMatch) {
    //                 return setNewState(END_MATCH);
    //             }

    //             break;
    //         }
    //     }
    // }

    // Match
    void setNewState(State newState) {
        currentState = newState;
        Logging::println("New state: %s", stateToStr(currentState));

        switch (currentState) {
            case GO_TO_POS0: {
                side = Switchers::getSide() ? 1 : 0;
                Logging::println("Loaded side %i", side);

                RobotPose* robotPose = this->control->getRobotPose();

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

                Goal goal(targetPos.x, targetPos.y, Goal::COORD);
                this->control->setCurrentGoal(goal);
                Logging::println("Go to: %f %f", targetPos.x, targetPos.y);

                break;
            }

            case TURN_IN_POS0: {
                float targetAngle = positions[1][side].theta;
                Logging::println("target angle: %f", targetAngle);

                Goal goal(targetAngle, Goal::ANGLE);
                this->control->setCurrentGoal(goal);

                break;
            }

            case GO_TO_POS1: {
                Pose targetPos = positions[2][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

                Goal goal(targetPos.x, targetPos.y, Goal::Direction::BACKWARD);
                this->control->setCurrentGoal(goal);
                Logging::println("Go to: %f %f", targetPos.x, targetPos.y);

                break;
            }

            case LIGHTHOUSE: {
                // TODO : bourrage
                Logging::println("TODO : bourrage");
                Goal goal(0., 50., Goal::CIRCULAR);
                this->control->setCurrentGoal(goal);
            }

            case GO_TO_POS2: {
                Pose targetPos = positions[3][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

                Goal goal(targetPos.x, targetPos.y, Goal::Direction::FORWARD);
                this->control->setCurrentGoal(goal);
                Logging::println("Go to: %f %f", targetPos.x, targetPos.y);

                break;
            }

            case TURN_IN_POS2: {
                float targetAngle = positions[4][side].theta;
                Logging::println("target angle: %f", targetAngle);

                Goal goal(targetAngle, Goal::ANGLE);
                this->control->setCurrentGoal(goal);

                break;
            }

            case GO_TO_POS3: {
                Pose targetPos = positions[5][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

                Goal goal(targetPos.x, targetPos.y);
                this->control->setCurrentGoal(goal);
                Logging::println("Go to: %f %f", targetPos.x, targetPos.y);

                break;
            }

            case TURN_IN_POS3: {
                float targetAngle = positions[6][side].theta;
                Logging::println("target angle: %f", targetAngle);

                Goal goal(targetAngle, Goal::ANGLE);
                this->control->setCurrentGoal(goal);

                break;
            }

            case GO_TO_POS4: {
                Pose targetPos = positions[7][side];
                Logging::println("target pos x: %f y: %f", targetPos.x, targetPos.y);

                Goal goal(targetPos.x, targetPos.y);
                this->control->setCurrentGoal(goal);
                Logging::println("Go to: %f %f", targetPos.x, targetPos.y);

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
