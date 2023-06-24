#ifndef _CORE_H_
#define _CORE_H_

#include "states.h"
#include "tasks.h"
#include "mywebserver.h"

void handle_cmd(cmd_t inc_cmd);

extern TaskHandle_t tLoop;
extern TaskHandle_t tTim;
extern TaskHandle_t tDim;
extern bool tim_alive;
extern bool dim_alive;
extern bool led_alive;
extern bool bell;
extern cmd_t last_cmd;

#endif