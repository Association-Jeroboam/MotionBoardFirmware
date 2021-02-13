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
#include <stdlib.h>

#include "hal.h"
#include "chprintf.h"
#include "shell.h"
#include "BuildConf.hpp"
#include <cstring>
#include "DataStreamer.hpp"
#include "Logging.hpp"
#include "MotorControlLoop.hpp"

char **endptr;

/*
 * Shell history buffer
 */
char history_buffer[SHELL_MAX_HIST_BUFF];

/*
 * Shell completion buffer
 */
char *completion_buffer[SHELL_MAX_COMPLETIONS];

/*
 * Shell commands
 */
static void cmd_data_stream(BaseSequentialStream *chp, int argc, char *argv[]) {

    (void)chp;
    if (argc == 1) {
        if (!strcmp(argv[0], "start")) {
            DataStreamer::instance()->start(NORMALPRIO);
            return;
        } else if (!strcmp(argv[0], "stop")) {
            DataStreamer::instance()->getSelfX().requestTerminate();
            return;
        }
    }

    Logging::println("usage:");
    Logging::println("data_stream [start/stop]");
}

static void cmd_motor(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void)chp;
    if(argc >= 3){
        enum Board::IO::motor motor;
        if (!strcmp(argv[0], "left")) {
            motor = Board::IO::LEFT_MOTOR;
        } else if (!strcmp(argv[0], "right")) {
            motor = Board::IO::RIGHT_MOTOR;
        } else{
            goto usage;
        }

        if (!strcmp(argv[1], "speed")) {
            float speed = atof(argv[2]);
            MotorControlLoop::instance()->motorSetSpeed(motor, speed);
            return;
        } else if (!strcmp(argv[1], "pid")) {
            float p = 0.;
            float i = 0.;
            float d = 0.;
            float * coeffs[3] = {&p, &i, &d};

            for(uint8_t i = 0; i < argc - 2; i++){
                *coeffs[i] = atof(argv[i + 2]);
            }

            MotorControlLoop::instance()->motorSetPID(motor, p, i, d);
            return;
        } else {
            goto usage;
        }
    }

usage:
    Logging::println("usage:");
    Logging::println("motor [left/right] [pid/speed] [parameters]");
}

static const ShellCommand commands[] = {
        {"data_stream", cmd_data_stream},
        {"motor", cmd_motor},
        {NULL, NULL}};

/*
 * Shell configuration
 */
ShellConfig shell_cfg = {
        (BaseSequentialStream *) &SHELL_DRIVER,
        commands,
        history_buffer,
        sizeof(history_buffer),
        completion_buffer};

/** @} */
