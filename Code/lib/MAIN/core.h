#ifndef _CORE_H_
#define _CORE_H_

#include <Arduino.h>
#include "states.h"

/// @brief Flag enumeration for flag selection and setting.
typedef enum flag_t{
    f_timer_alive,
    f_dimmer_alive,
    f_led_alive,
    f_bell_alive,
    NUM_FLAGS
}flag_t;

/// @brief Task enumeration for handle selection
typedef enum task_t{
    t_loop,
    t_tim,
    t_dim,
    t_led,
    NUM_TASKS
}task_t;

/// @brief Command struct with content and place of origin
typedef struct cmd_t{
    uint8_t origin;
    uint8_t content;
    uint16_t pad;
} cmd_t;

/// @brief Parse command and process it. Checks state of
/// core and starts worker tasks if necessary.
/// @param inc_cmd cmd_t, command to handle
void handle_cmd(cmd_t inc_cmd);

/// @brief Obtain a specific flag from the core without
/// accessing it directly.
/// @param f flag_t, flag to read
/// @return bool, status of flag
bool get_flag(flag_t f);

/// @brief Set a specific flag.
/// @param f flag_t, flag to set
/// @param mode bool, state of flag to set
void set_flag(flag_t f, bool mode);

/// @brief Get the task handle for a task.
/// @param t task_t, task type to get
/// @return TaskHandle_t*, pointer to handle of task
/// @note FreeRTOS expects the pointer to a task handle
/// in xTaskCreate(), but the handle itself for all other
/// functionalities. This makes the return of this function
/// a double pointer, to be exact void**. Don't forget to
/// dereference its return value when passing it to functions
/// like xTaskNotify().
TaskHandle_t* get_task_handle(task_t t);

extern cmd_t last_cmd;

#endif