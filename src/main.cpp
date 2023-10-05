// Select your preferred example and flash the project
// The preprocessor-defines are verbose on purpose to illustrate the usage of this file
#include <Arduino.h>
#include "examples.h"

#define EXAMPLE_TASK_SPAM // uncomment this line if you want to run `example_task_control_IR.cpp`
#ifndef EXAMPLE_TASK_SPAM
#define EXAMPLE_TASK_CONTROL_IR
#endif

void setup(){
    #ifdef EXAMPLE_TASK_SPAM
    setup_example_task_spam();
    #endif

    #ifdef EXAMPLE_TASK_CONTROL_IR
    setup_example_task_control_IR();
    #endif
}

void loop(){
    // Nothing to do here! Tasks run in their own loop.
}