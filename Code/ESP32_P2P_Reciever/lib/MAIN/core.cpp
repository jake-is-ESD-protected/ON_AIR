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

extern TaskHandle_t tWebserver;
c_core core;


c_core::c_core()
{

}


void c_core::handle_cmd(cmd_t inc_cmd)
{
    /*  
        button has been pressed:
        - proceed to send message to master
        - blink LED for RING_TIME ms
    */
    if((inc_cmd.content == STATE_BELL) && !tim_alive)
    {
        dim_alive = false;

        cmd_t c = {
            .origin = ORG_SW,
            .content = STATE_BELL
        };
        mbox.push(c, false);
        mbox.notify(tWebserver, false);
        
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
        tim_alive = false;
    }

    /*  
        any action taken while device is entering dim-mode:
        - delete dim-timer
    */
    if((inc_cmd.content != STATE_IDLE) && dim_alive)
    {
        dim_alive = false;
    }

    lcd.display_state(inc_cmd.content);
    volatile_state = inc_cmd.content;
    // Serial.printf("\ttim-task: %d, led-task: %d, dim-task: %d\r\n", tim_alive, led_alive, dim_alive);

    if(inc_cmd.content < STATE_TRANS_BORDER_C)          
    {   // skip any instances of transitional states
        Serial.printf("[DEBUG]\tentering state '%d'\r\n", inc_cmd.content);
        last_cmd = inc_cmd;
    }
}