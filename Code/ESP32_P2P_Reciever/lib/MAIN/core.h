#ifndef _CORE_H_
#define _CORE_H_

#include "mailbox.h"
#include "states.h"
#include "mywebserver.h"

class c_core
{
public:

    bool tim_alive = false;
    bool dim_alive = false;
    bool led_alive = false;

    TaskHandle_t tTim = NULL;
    TaskHandle_t tDim = NULL;


    cmd_t last_cmd = {
        .origin = ORG_SW,
        .content = STATE_IDLE
    };

    uint8_t volatile_state = STATE_IDLE;


    c_core();

    void handle_cmd(cmd_t inc_cmd);

    private:

};

extern c_core core;
extern mailbox mbox;
extern TaskHandle_t tLoop;

#endif