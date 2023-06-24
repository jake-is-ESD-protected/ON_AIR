/*
*****************************************************************************************
source name:        core.cpp
auth:               Jakob T.
date:               22
brief:              fsm
version:            V1.0
*****************************************************************************************
*/

#include "LiquidCrystal_I2C.h"
#include "core.h"
#include "states.h"
#include "hardware.h"
#include "lcd.h"
#include "tasks.h"

c_core core;


c_core::c_core()
{

}


void c_core::handle_cmd(cmd_t inc_cmd)
{
    /*  
        button has been pressed:
        - blink LED for RING_TIME ms
        - wait for client to pick up the bell-status
    */
    if((inc_cmd.content == STATE_BELL) && !tim_alive)
    {
        bell = true;
        Serial.printf("[CORE]\tIn bell-handler\n");
        dim_alive = false;

        xTaskCreate(time_led_task, 
            "start a timer which steers blinking for 5 s",
            1024,
            NULL,
            1,
            &tTim);       
    }

    /*  
        device is in idle mode and has not yet started the dim-timer:
        - launch dimming-task
    */
    if(inc_cmd.content == STATE_IDLE && !dim_alive)
    {
        Serial.printf("[CORE]\tLaunch dimming task\n");
        dim_alive = true;
        xTaskCreate(dim_lcd_task,
                    "go into power saving mode",
                    2048,
                    NULL,
                    1,
                    &tDim);
    }

    /*  
        cancel ringing task if answer occurs from master
    */
    if((inc_cmd.content < STATE_TRANS_BORDER_C) && tim_alive)
    {
        Serial.printf("[CORE]\tRegistered answer from client\n");
        tim_alive = false;
    }

    /*  
        any action taken while device is entering dim-mode:
        - delete dim-timer
    */
    if((inc_cmd.content != STATE_IDLE) && dim_alive)
    {
        Serial.println("[CORE]\tStop dimming task\n");
        dim_alive = false;
    }

    lcd.display_state(inc_cmd.content);
    // Serial.printf("\ttim-task: %d, led-task: %d, dim-task: %d\r\n", tim_alive, led_alive, dim_alive);

    if(inc_cmd.content < STATE_TRANS_BORDER_C)          
    {   // skip any instances of transitional states
        Serial.printf("[CORE]\tentering state '%d'\r\n", inc_cmd.content);
        last_cmd = inc_cmd;
    }
}