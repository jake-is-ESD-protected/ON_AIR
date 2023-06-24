#ifndef _TASKS_H_
#define _TASKS_H_

#include "Arduino.h"

void mainLoopTask(void* param);


void webserverTask(void* param);


void wsClientHandlerTask(void* param);


void time_led_task(void* param);


void blink_led_task(void* param);


void dim_lcd_task(void* param);


extern TaskHandle_t tLoop;
extern TaskHandle_t tWebserver;
extern TaskHandle_t tClientHandler;

#endif