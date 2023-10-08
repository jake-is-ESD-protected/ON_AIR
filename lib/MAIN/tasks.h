#ifndef _TASKS_H_
#define _TASKS_H_

#include "Arduino.h"
#include "core.h"

/// @brief Task for main loop, replaces Arduino "loop()"
/// @param param void*, optional parameters
void mainLoopTask(void* param);

/// @brief Task for everything webserver related
/// @param param void*, optional parameters
void webserverTask(void* param);

/// @brief Task for fixed time return of LED blinking task
/// @param param void*, optional parameters
void time_led_task(void* param);

/// @brief Task for periodically blinking the bell LED
/// @param param void*, optional parameters
void blink_led_task(void* param);

/// @brief Task for fixed time poweroff of the display
/// @param param void*, optional parameters
void dim_lcd_task(void* param);

/// @brief Task notification wrapper for FreeRTOS "xTaskNotify()"
/// @param t Taskhandle_t, handle to task to be notified
/// @param c cmd_t, command to be sent
/// @param from_isr bool, specifies ISR or non-ISR origin
void IRAM_ATTR task_notify(TaskHandle_t t, cmd_t c, bool from_isr);

/// @brief Task notification wrapper for FreeRTOS "ulTaskNotifyTake"
/// @param ticks_to_wait TickType_t, timeout time, use "portMAX_DELAY" for blocking
cmd_t IRAM_ATTR task_notify_take(TickType_t ticks_to_wait);

#endif