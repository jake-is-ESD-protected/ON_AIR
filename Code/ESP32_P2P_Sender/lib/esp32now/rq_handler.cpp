/*
*****************************************************************************************
source name:        rq_handelr.cpp
auth:               Jakob T.
date:               25.12.21
brief:              hardware access bridge from higher up requests
version:            V1.1
*****************************************************************************************
*/

#include "rq_handler.h"

static int actual_state = STATE_IDLE;
static int targeted_state = STATE_NO_STATE;

flag_t dim_alive{.stat = false, .name = DIM_FLAG};
flag_t led_alive{.stat = false, .name = LED_FLAG};

TaskHandle_t tDim = NULL;

void handle_ESPnow_output(esp_now_send_status_t* status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(*status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


uint8_t handle_cmd(cmd_t inc_cmd)
{
    set_flag(dim_alive, false);
    switch(inc_cmd.content)
    {
        case PUSH:
            actual_state = targeted_state;
            break;

        case LEFT:
            if(targeted_state >= MAX_STATE)
            {
                targeted_state = STATE_IDLE;
            }
            else
            {
                targeted_state += 1;
            }
            break;
    
        case RIGHT:
            if(targeted_state <= STATE_IDLE)
            {
                targeted_state = STATE_WELC;
            }
            else
            {
                targeted_state -= 1;
            }
            break;

        case BELL_INT:
            targeted_state = STATE_BELL;
            actual_state = targeted_state;
            // kickstart a blinking-thread here
            break;

        case SLEEP:
            clear_oled();
            break;

        default:
            break;
    }

    // handle software
    if(targeted_state != actual_state)
    {
        drawbmp(targeted_state, false);
    }
    else
    {
        drawbmp(actual_state, true);
        if((actual_state != STATE_BELL) && (inc_cmd.content == PUSH))
        {
            esp_send((uint8_t)actual_state);
        }
    }
    
    set_flag(dim_alive, true);
    xTaskCreate(dim_task, "dim task", 2048, NULL, 1, &tDim);

    return targeted_state;
}


void dim_task(void* param)
{
    uint32_t i = 0;
    Serial.printf("hello from dim task! dim alive: %d\n", dim_alive.stat);
    while(dim_alive.stat && i < POWER_SAVE_TIME)
    {
        vTaskDelay(POWER_SAVE_TIME_TICK / portTICK_PERIOD_MS);
        i++;
    }
    Serial.printf("times up! %d\n", i);

    // device was touched before sleep time up
    if(i < POWER_SAVE_TIME)
    {
        vTaskDelete(NULL);
    }

    // device enters sleep mode
    else
    {
        mailbox_push({.origin=ORG_SW, .content=SLEEP}, false);
        set_flag(dim_alive, false);
        vTaskDelete(NULL);
    }
}


void set_flag(flag_t flag, bool tf)
{    
    flag.stat = tf;
    Serial.printf("[FLAG]   set flag <%d> to %d\r\n", flag.name, flag.stat);
}

