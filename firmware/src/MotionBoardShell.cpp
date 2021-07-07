/*
    Copyright (C) 2016 Jonathan Struebel

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    common/shellcfg.c
 * @brief   CLI shell config.
 *
 * @addtogroup SHELL
 * @{
 */

#include "BuildConf.hpp"
#include "ControlThread.hpp"
#include "DataStreamer.hpp"
#include "Logging.hpp"
#include "MotionBoard.hpp"
#include "Peripherals.hpp"
#include "chprintf.h"
#include "hal.h"
#include "shell.h"
#include <cstring>
#include <stdlib.h>

char** endptr;

/*
 * Shell history buffer
 */
char history_buffer[SHELL_MAX_HIST_BUFF];

/*
 * Shell completion buffer
 */
char* completion_buffer[SHELL_MAX_COMPLETIONS];

/*
 * Shell commands
 */
static void cmd_data_stream(BaseSequentialStream* chp, int argc, char* argv[]) {
    static bool thread_launched = false;
    (void)chp;
    if (argc == 1) {
        if (!strcmp(argv[0], "start")) {
            if (!thread_launched) {
                DataStreamer::instance()->start(NORMALPRIO);
                thread_launched = true;
            }
            return;
        } else if (!strcmp(argv[0], "stop")) {
            if (thread_launched) {
                DataStreamer::instance()->getSelfX().requestTerminate();
                thread_launched = false;
            }
            return;
        }
    }

    Logging::println("usage:");
    Logging::println("data_stream [start/stop]");
}

static void cmd_motor(BaseSequentialStream* chp, int argc, char* argv[]) {
    (void)chp;
    if (argc >= 3) {
        Peripherals::Motor motor;
        if (!strcmp(argv[0], "left")) {
            motor = Peripherals::LEFT_MOTOR;
        } else if (!strcmp(argv[0], "right")) {
            motor = Peripherals::RIGHT_MOTOR;
        } else {
            goto usage;
        }

        if (!strcmp(argv[1], "duty_cycle")) {
            float duty_cycle = atof(argv[2]);
            float dc_left;
            float dc_right;

            if (motor == Peripherals::Motor::LEFT_MOTOR) {
                dc_left  = duty_cycle;
                dc_right = 0;
            } else {
                dc_left  = 0;
                dc_right = duty_cycle;
            }

            Goal goal(dc_left, dc_right, Goal::PWM);
            ControlThread::instance()->getControl()->setCurrentGoal(goal);

            return;
        } else {
            goto usage;
        }
    }

usage:
    Logging::println("usage:");
    Logging::println("motor [left/right] duty_cycle [duty_cycle]");
}

static void cmd_control(BaseSequentialStream* chp, int argc, char* argv[]) {
    (void)chp;
    if (argc >= 1) {
        if (!strcmp(argv[0], "polar") && argc == 3) {
            float angle    = atof(argv[1]);
            float distance = atof(argv[2]);
            Logging::println("polar %f %f", angle, distance);
            Goal goal(angle, distance, Goal::POLAR);
            ControlThread::instance()->getControl()->setCurrentGoal(goal);
            return;
        } else if (!strcmp(argv[0], "angle_pid") && argc == 4) {
            float kp = atof(argv[1]);
            float ki = atof(argv[2]);
            float kd = atof(argv[3]);
            Logging::println("angle_pid %f %f %f", kp, ki, kd);
            ControlThread::instance()->getControl()->setAnglePID(kp, ki, kd);
            return;
        } else if (!strcmp(argv[0], "distance_pid") && argc == 4) {
            float kp = atof(argv[1]);
            float ki = atof(argv[2]);
            float kd = atof(argv[3]);
            Logging::println("distance pid %f %f %f", kp, ki, kd);
            ControlThread::instance()->getControl()->setDistancePID(kp, ki, kd);
            return;
        } else if (!strcmp(argv[0], "circular") && argc == 3) {
            float angularSpeed = atof(argv[1]);
            float linearSpeed  = atof(argv[2]);
            Goal  goal(angularSpeed, linearSpeed, Goal::CIRCULAR);
            ControlThread::instance()->getControl()->setCurrentGoal(goal);
            return;
        } else if (!strcmp(argv[0], "reset")) {
            ControlThread::instance()->getControl()->reset();
            return;
        }
    }

    Logging::println("usage:");
    Logging::println("control [polar] [angle] [distance]");
    Logging::println("control [angle_pid/distance_pid] [p] [i] [d]");
    Logging::println("control circular [angSpd] [linSpd]");
}
static void cmd_pliers(BaseSequentialStream* chp, int argc, char* argv[]) {
    (void)chp;
    if (argc == 2) {
        enum pliersID    id = (enum pliersID)atoi(argv[0]);
        enum pliersState state;
        if (!strcmp(argv[1], "open")) {
            state = PLIERS_OPEN;
        } else if (!strcmp(argv[1], "close")) {
            state = PLIERS_CLOSE;
        } else {
            goto usage;
        }

        if (id > PLIERS_REAR_FAR_LEFT) {
            Logging::println("Bad ID");
            goto usage;
        }

        canFrame_t frame = {
            .ID  = CAN_PLIERS_ID,
            .len = CAN_PLIERS_LEN,
        };
        frame.data.pliersData = {
            .plierID = id,
            .state   = state,
        };
        Board::Com::CANBus::send(frame);
    } else {
        goto usage;
    }
    return;

usage:
    Logging::println("usage:");
    Logging::println("pliers [pliersID] [open/close]");
}

static void cmd_pliers_block(BaseSequentialStream* chp, int argc, char* argv[]) {
    (void)chp;
    if (argc == 1) {
        uint8_t state;
        if (!strcmp(argv[0], "engage")) {
            state = 1;
        } else if (!strcmp(argv[0], "disengage")) {
            state = 0;
        } else {
            goto usage;
        }

        canFrame_t frame = {
            .ID  = CAN_PLIERS_BLOCK_ID,
            .len = CAN_PLIERS_BLOCK_LEN,
        };
        frame.data.pliersBlockData = {
            .state = state,
        };
        Board::Com::CANBus::send(frame);
    } else {
        goto usage;
    }
    return;

usage:
    Logging::println("usage:");
    Logging::println("pliers_block [engage/disengage]");
}

static void cmd_slider(BaseSequentialStream* chp, int argc, char* argv[]) {
    (void)chp;
    if (argc == 2) {
        uint16_t   distance = atoi(argv[1]);
        canFrame_t frame    = {
            .ID  = CAN_SLIDERS_ID,
            .len = CAN_SLIDERS_LEN,
        };
        frame.data.slidersData = {
            .position = distance,
        };

        if (!strcmp(argv[0], "elevator")) {
            frame.data.slidersData.sliderID = SLIDER_ELEVATOR;
        } else if (!strcmp(argv[0], "translator")) {
            frame.data.slidersData.sliderID = SLIDER_ELEVATOR;
        } else {
            goto usage;
        }
        Board::Com::CANBus::send(frame);
    } else {
        goto usage;
    }
    return;

usage:
    Logging::println("usage:");
    Logging::println("slider [elevator/translator] [distance (mm)]");
}

static const ShellCommand commands[] = {
    {"data_stream", cmd_data_stream},
    {"motor", cmd_motor},
    {"control", cmd_control},
    {"pliers", cmd_pliers},
    {"pliers_block", cmd_pliers_block},
    {"slider", cmd_slider},
    {NULL, NULL}};

/*
 * Shell configuration
 */
ShellConfig shell_cfg = {
    (BaseSequentialStream*)&SHELL_DRIVER,
    commands,
    history_buffer,
    sizeof(history_buffer),
    completion_buffer};

/** @} */
