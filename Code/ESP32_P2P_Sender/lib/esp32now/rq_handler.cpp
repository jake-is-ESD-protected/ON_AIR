/*
*****************************************************************************************
source name:        rq_handelr.cpp
auth:               Jakob T.
date:               25.12.21
brief:              hardware access bridge from higher up requests
version:            V1.0
IMPORTANT:          THIS AND ONLY THIS FILE HAS PERMISSION TO WRITE TO HARDWARE
                    This includes: OLED, Serial, LED
*****************************************************************************************
*/

#include "rq_handler.h"

bool bell_ringing = false;

void handle_input(char* cmd, int len)
{
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print(cmd);
    if(!strcmp(cmd, ID_BELL_INT))
    {
        bell_ringing = true;
        digitalWrite(LED_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_PIN, LOW);
    }
    else
    {
        bell_ringing = false;
    }
}


void handle_output(esp_now_send_status_t* status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(*status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}