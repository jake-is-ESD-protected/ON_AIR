#include "LiquidCrystal_I2C.h"
#include "core.h"
#include "tasks.h"
#include "hardware.h"
#include "lcd.h"

bool flags[NUM_FLAGS] = {false};
TaskHandle_t task_handles[NUM_TASKS] = {NULL};

cmd_t last_cmd = {
    .origin = ORG_SW,
    .content = STATE_IDLE
};


void handle_cmd(cmd_t inc_cmd)
{
    TaskHandle_t timer = get_task_handle(t_tim);
    TaskHandle_t dimmer = get_task_handle(t_dim);
    /*  
        button has been pressed:
        - blink LED for RING_TIME ms
        - wait for client to pick up the bell-status
    */
    if((inc_cmd.content == STATE_BELL) && !get_flag(f_timer_alive)){
        Serial.printf("[CORE]\tIn bell-handler\n");
        set_flag(f_bell_alive, true);
        set_flag(f_dimmer_alive, false);

        xTaskCreate(time_led_task, 
            "start a timer which steers blinking for 5 s",
            2048,
            NULL,
            1,
            &timer);       
    }

    /*  
        device is in idle mode and has not yet started the dim-timer:
        - launch dimming-task
    */
    if(inc_cmd.content == STATE_IDLE && !get_flag(f_dimmer_alive)){
        Serial.printf("[CORE]\tLaunch dimming task\n");
        set_flag(f_dimmer_alive, true);
        xTaskCreate(dim_lcd_task,
                    "go into power saving mode",
                    2048,
                    NULL,
                    1,
                    &dimmer);
    }

    /*  
        cancel ringing task if answer occurs from master
    */
    if((inc_cmd.content < STATE_TRANS_BORDER_C) && get_flag(f_timer_alive)){
        Serial.printf("[CORE]\tRegistered answer from client\n");
        set_flag(f_timer_alive, true);
    }

    /*  
        any action taken while device is entering dim-mode:
        - delete dim-timer
    */
    if((inc_cmd.content != STATE_IDLE) && get_flag(f_dimmer_alive))
    {
        Serial.println("[CORE]\tStop dimming task\n");
        set_flag(f_dimmer_alive, false);
    }

    lcd.display_state(inc_cmd.content);

    if(inc_cmd.content < STATE_TRANS_BORDER_C)          
    {   // skip any instances of transitional states
        Serial.printf("[CORE]\tentering state '%d'\r\n", inc_cmd.content);
        last_cmd = inc_cmd;
    }
}

bool get_flag(flag_t f){
    // assert something idk
    return flags[f];
}

void set_flag(flag_t f, bool mode){
    // assert something idk
    flags[f] = mode;
}

TaskHandle_t* get_task_handle(task_t t){
    // assert something idk
    return &task_handles[t];
}