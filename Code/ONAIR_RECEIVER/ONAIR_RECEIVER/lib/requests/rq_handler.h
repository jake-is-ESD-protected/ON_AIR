#ifndef _RQ_HANDLER_H_
#define _RQ_HANDLER_H_

#include "esp32now.h"
#include "hardware.h"
#include "mailbox.h"


void handle_ESPnow_output(esp_now_send_status_t* status);


void handle_cmd(cmd_t inc_cmd);


void time_led_task(void* param);


void blink_led_task(void* param);


void dim_lcd_task(void* param);

extern mailbox mbox;
extern TaskHandle_t tLoop;

#endif