#ifndef _RQ_HANDLER_H_
#define _RQ_HANDLER_H_

#include "mailbox.h"
#include "esp_now.h"


void handle_cmd(cmd_t inc_cmd);


void time_led_task(void* param);


void blink_led_task(void* param);


void dim_lcd_task(void* param);

extern mailbox mbox;
extern TaskHandle_t tLoop;

#endif