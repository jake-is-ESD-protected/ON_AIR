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

void handle_ESPnow_input(char* cmd, int len)
{
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print(cmd);
    if(!strcmp(cmd, ID_BELL_INT))
    {
        bell_ringing = true;
        digitalWrite(LED_RED, HIGH);
        delay(1000);
        digitalWrite(LED_RED, LOW);
    }
    else
    {
        bell_ringing = false;
    }
}


void handle_ESPnow_output(esp_now_send_status_t* status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(*status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


int handle_hw_cmd(int hw_cmd)
{
    switch(hw_cmd)
    {
        case PUSH:
            actual_state = targeted_state;
            drawbmp(actual_state, true);
            return targeted_state;

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

        default:
            break;
    }
    if(targeted_state == actual_state)
    {
        drawbmp(actual_state, true);
    }
    else
    {
        drawbmp(targeted_state, false);
    }
    return targeted_state;
}

