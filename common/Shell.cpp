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
#include "MotorControl.hpp"

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
static bool thread_launched = false;
    (void)chp;
    if (argc == 1) {
        if (!strcmp(argv[0], "start")) {
            if(! thread_launched) {
                DataStreamer::instance()->start(NORMALPRIO);
                thread_launched = true;
            }
            return;
        } else if (!strcmp(argv[0], "stop")) {
            if(thread_launched) {
                DataStreamer::instance()->getSelfX().requestTerminate();
                thread_launched = false;
            }
            return;
        }
    }

    Logging::println("usage:");
    Logging::println("data_stream [start/stop]");
}

static void cmd_motor(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void)chp;


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
