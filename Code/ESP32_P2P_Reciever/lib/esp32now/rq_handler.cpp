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
extern portMUX_TYPE mux;

bool tim_alive = false;
bool dim_alive = false;
bool led_alive = false;


static cmd_t last_cmd = {
    .origin = ORG_NOW,
    .content = STATE_IDLE
};

void handle_cmd(cmd_t inc_cmd)
{
    TaskHandle_t tTim = NULL;
    TaskHandle_t tDim = NULL;

    /*  
        button has been pressed:
        - proceed to send message to master
        - blink LED for RING_TIME ms
    */
    if((inc_cmd.content == STATE_BELL) && !tim_alive)
    {
        dim_alive = false;

        esp_err_t err = esp_send((uint8_t)BELL_INT);
        if(err != ESP_OK)
        {
            Serial.printf("failed to send data: %d\r\n", inc_cmd.content);
        }
        
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

    lcd_display_state(inc_cmd.content);
    Serial.printf("\ttim-task: %d, led-task: %d, dim-task: %d\r\n", tim_alive, led_alive, dim_alive);

    if(inc_cmd.content < STATE_TRANS_BORDER_C)          
    {   // skip any instances of transitional states
        Serial.printf("\tlast_cmd '%d' overwritten by '%d'\r\n", last_cmd.content, inc_cmd.content);
        last_cmd = inc_cmd;
    }
}


void handle_ESPnow_output(esp_now_send_status_t* status)
{
    Serial.print("\tLast Packet Send Status:\t");
    Serial.println(*status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void time_led_task(void* param)
{
    tim_alive = true;
    led_alive = true;
    TaskHandle_t tLED = NULL;
    xTaskCreate(blink_led_task,
                    "blink green LED",
                    1024,
                    NULL,
                    1,
                    &tLED);

    // check periodically if timer is not cancelled from outside       
    uint8_t i = 0;
    while(tim_alive && (i < RING_AMOUNT))
    {
        vTaskDelay((RING_TIME / RING_AMOUNT) / portTICK_PERIOD_MS);
        i++;
    }
    if(!tim_alive)
    {
        // process forced to exit by outside means
        led_alive = false;
        vTaskDelete(NULL);
    }
    else
    {
        // process tasked to die after time is up
        led_alive = false;
        mailbox_push({.origin = ORG_SW, .content = STATE_ATTRIBUTE_LA_OFF}, false);
        
        cmd_t cmd = {
            .origin = ORG_SW,
            .content = STATE_NO_RESPONSE
        };
        mailbox_push(cmd, false);
        vTaskDelay(DOUBLE_MSG_DELAY * 2 / portTICK_PERIOD_MS);
        cmd = last_cmd;
        mailbox_push(cmd, false);
        tim_alive = false;
        vTaskDelete(NULL);
    }
}


void blink_led_task(void* param)
{
    while(led_alive)
    {
        for(int i = 0; i < 3; i++)
        {
            led_on();
            vTaskDelay(100 / portTICK_PERIOD_MS);
            led_off();
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}


void dim_lcd_task(void* param)
{
    // check periodically if dimmer is not cancelled from outside       
    uint8_t i = 0;
    while(dim_alive && (i < POWER_SAVE_TIME_SPLIT))
    {
        vTaskDelay(((POWER_SAVE_TIME / POWER_SAVE_TIME_SPLIT) / portTICK_PERIOD_MS));
        i++;
    }
    if(!dim_alive)
    {
        vTaskDelete(NULL);
    }
    else
    {
        mailbox_push({.origin = ORG_SW, .content = STATE_ATTRIBUTE_LCD_DARK}, false);
        dim_alive = false;
        vTaskDelete(NULL);
    }
    
}