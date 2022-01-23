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

bool bell_ringing = false;
static int actual_state = STATE_IDLE;
static int targeted_state = 999; // dummy value


void handle_ESPnow_output(esp_now_send_status_t* status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(*status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


int handle_cmd(int inc_cmd)
{
    switch(inc_cmd)
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
            bell_ringing = true;
            targeted_state = STATE_BELL;
            actual_state = targeted_state;
            break;

        default:
            break;
    }
    if(targeted_state == actual_state)
    {
        // commit to state. Transfer data to slave
        drawbmp(actual_state, true);
        if(actual_state != STATE_BELL)
        {
            esp_send((uint8_t)actual_state);
        }
    }
    else
    {
        drawbmp(targeted_state, false);
    }
    return targeted_state;
}

