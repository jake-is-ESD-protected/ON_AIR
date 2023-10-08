// Select your preferred example and flash the project
// The preprocessor-defines are verbose on purpose to illustrate the usage of this file
#include <Arduino.h>
#include "examples.h"
#include "setup_main.h"

// uncomment just ONE of these to run them:
// #define EXAMPLE_TASK_SPAM
// #define EXAMPLE_TASK_CONTROL_IR
#define MAIN_APP

void setup(){
    #ifdef EXAMPLE_TASK_SPAM
    setup_example_task_spam();
    return;
    #endif

    #ifdef EXAMPLE_TASK_CONTROL_IR
    setup_example_task_control_IR();
    return;
    #endif

    #ifdef MAIN_APP
    setup_main();
    return;
    #endif
}

void loop(){
    // Nothing to do here! Tasks run in their own loop.
}