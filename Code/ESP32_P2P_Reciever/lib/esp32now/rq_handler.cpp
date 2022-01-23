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

extern QueueHandle_t qCMD;

void handle_cmd(uint8_t inc_cmd)
{
    if(inc_cmd == STATE_BELL)
    {
        esp_err_t err = esp_send((uint8_t)BELL_INT);
        if(err != ESP_OK)
        {
            Serial.printf("failed to send data: %d", inc_cmd);
        }
        while(inc_cmd == 0)
        {
            xQueueReceive(qCMD, &inc_cmd, 1);
        }
        
    }
    lcd_display_state((uint8_t)inc_cmd);
}


void handle_ESPnow_output(esp_now_send_status_t* status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(*status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
