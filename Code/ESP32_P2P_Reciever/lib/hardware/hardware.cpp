/*
*****************************************************************************************
source name:        hardware.cpp
auth:               Jakob T.
date:               16.01.22
brief:              hardware access
version:            V1.2
*****************************************************************************************
*/
#include "hardware.h"
#include "mailbox.h"
#include "states.h"
#include "mailbox.h"
#include "mainloop.h"


static unsigned long last_interrupt_time_BUT = 0;


void IRAM_ATTR BUT_ISR() 
{
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time_BUT > IR_DEBOUNCE_TIME)
    {
        cmd_t c = {
            .origin = ORG_HW,
            .content = STATE_BELL
        };
        mbox.push(c, true);
        mbox.notify(tLoop, true);
    }
    last_interrupt_time_BUT = interrupt_time;
}


void init_ISRs(void)
{
    attachInterrupt(BUTTON_IN_PIN, BUT_ISR, FALLING);
}


void init_gpios(void)
{
    pinMode(BUTTON_IN_PIN, INPUT);
    pinMode(LED_ARRAY, OUTPUT);
    pinMode(BUTTON_LED_PIN, OUTPUT);
}


void led_on(void)
{
    digitalWrite(BUTTON_LED_PIN, HIGH);
}


void led_off(void)
{
    digitalWrite(BUTTON_LED_PIN, LOW);
}


void led_blink(void)
{
    for(int k = 0; k < RING_TIME; k++)
    {
        for(int i = 0; i < 3; i++)
        {
            
            digitalWrite(BUTTON_LED_PIN, HIGH);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            digitalWrite(BUTTON_LED_PIN, LOW);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}