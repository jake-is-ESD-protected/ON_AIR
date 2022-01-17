/*
*****************************************************************************************
source name:        rq_handelr.cpp
auth:               Jakob T.
date:               25.12.21
brief:              hardware access bridge from higher up requests
version:            V1.0
*****************************************************************************************
*/

#include "rq_handler.h"

bool bell_ringing = false;
static int state = STATE_IDLE;
extern QueueHandle_t qCMD;

void handle_ESPnow_input(char* cur_cmd, int len)
{
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print(cur_cmd);
    int i = 0;
    while(strcmp(cur_cmd, &cmd[i][0]))
    {
        state = i;
        i++;
        if(i == N_CMDS)
        {
            state = STATE_ERROR;
            break;
        }
    }
    xQueueSend(qCMD, &state, portMAX_DELAY);
}


void handle_ESPnow_output(esp_now_send_status_t* status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(*status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
