#ifndef _RQ_HANDLER_H_
#define _RQ_HANDLER_H_

#include "hardware.h"
#include "esp32now.h"

#define DIM_FLAG    1
#define LED_FLAG    2

typedef struct{
    bool stat;
    uint8_t name;
}flag_t;


void handle_ESPnow_output(esp_now_send_status_t* status);


uint8_t handle_cmd(cmd_t inc_cmd);


void dim_task(void* param);


void set_flag(flag_t flag, bool tf);

#endif