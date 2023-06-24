#ifndef _TASKS_H_
#define _TASKS_H_

#include "Arduino.h"

// command data type
typedef struct {
    uint8_t origin;
    uint8_t content;
    uint16_t pad;
} cmd_t;

void mainLoopTask(void* param);
void webserverTask(void* param);
void wsClientHandlerTask(void* param);
void time_led_task(void* param);
void blink_led_task(void* param);
void dim_lcd_task(void* param);
void IRAM_ATTR task_notify(TaskHandle_t t, cmd_t c, bool from_isr);
cmd_t IRAM_ATTR task_notify_take(TickType_t ticks_to_wait);

extern TaskHandle_t tLoop;
extern TaskHandle_t tWebserver;
extern TaskHandle_t tClientHandler;

#endif