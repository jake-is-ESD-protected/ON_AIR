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

// each task receives an alive-flag
bool tim_alive = false;
bool led_alive = false;
bool dim_alive = false;

static cmd_t last_cmd = {
    .origin = ORG_NOW,
    .content = STATE_IDLE
};

void handle_cmd(cmd_t inc_cmd)
{
    TaskHandle_t tTim = NULL;
    TaskHandle_t tLED = NULL;
    TaskHandle_t tDim = NULL;

    /*  
        button has been pressed:
        - proceed to send message to master
        - blink LED for RING_TIME ms
    */
    if(inc_cmd.content == STATE_BELL && !tim_alive)
    {
        esp_err_t err = esp_send((uint8_t)BELL_INT);
        if(err != ESP_OK)
        {
            Serial.printf("failed to send data: %d\r\n", inc_cmd.content);
        }
        xTaskCreate(time_led_task, 
                "start a timer which steers blinking for 5 s",
                1024,
                tLED,   // pass handle of child-thread
                1,
                &tTim);      
    }

    /*  
        button has been pressed:
        proceed to send message to master
        blink LED for RING_TIME ms
    */
    if(inc_cmd.content != STATE_NO_RESPONSE && inc_cmd.content != STATE_BELL)
    {
        if(tim_alive)
        {
            vTaskDelete(tTim); // program gets stuck here???
        }
        if(led_alive)
        {
            vTaskDelete(tLED);
        }
    }

    /*  
        device is in idle mode and has not yet started the dim-timer:
        - launch dimming-task
    */
    if(inc_cmd.content == STATE_IDLE && !dim_alive)
    {
        xTaskCreate(dim_lcd_task,
                    "go into power saving mode",
                    2048,
                    NULL,
                    1,
                    &tDim);
    }

    /*  
        any action taken while device is entering dim-mode:
        - delete dim-timer
    */
    if(inc_cmd.content != STATE_IDLE && dim_alive)
    {
        dim_alive = false;
        
        vTaskDelete(tDim);
        Serial.printf("killed dim-task\r\n");
    }

    lcd_display_state(inc_cmd.content);

    if(inc_cmd.content != STATE_NO_RESPONSE && inc_cmd.content != STATE_BELL)
    {   // skip any instances of the no response state or
        // bell state since they should not be returned to
        Serial.printf("last_cmd '%d' overwritten by '%d'\r\n", last_cmd.content, inc_cmd.content);
        last_cmd = inc_cmd;
    }

    Serial.printf("tim-task: %d, led-task: %d, dim-task: %d\r\n", tim_alive, led_alive, dim_alive);
}


void handle_ESPnow_output(esp_now_send_status_t* status)
{
    Serial.print("Last Packet Send Status:\t");
    Serial.println(*status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void time_led_task(void* param)
{
    tim_alive = true;
    TaskHandle_t tLED = (TaskHandle_t)param;
    xTaskCreate(toggle_led_task, 
                "toggle LED for bell-application",
                1024,
                NULL,
                1,
                &tLED);
    led_alive = true;            

    vTaskDelay(RING_TIME / portTICK_PERIOD_MS);

    vTaskDelete(tLED);
    mailbox_push({.origin = ORG_SW, .content = STATE_ATTRIBUTE_LA_OFF}, false);
    led_alive = false;
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


void toggle_led_task(void* param)
{
    while(1)
    {
        for(int i = 0; i < 3; i++)
        {
            
            mailbox_push({.origin = ORG_SW, .content = STATE_ATTRIBUTE_BL_ON}, false);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            mailbox_push({.origin = ORG_SW, .content = STATE_ATTRIBUTE_BL_OFF}, false);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void dim_lcd_task(void* param)  // TASK NOT GETTING KILLED!!!
{
    dim_alive = true;
    vTaskDelay(POWER_SAVE_TIME / portTICK_PERIOD_MS);
    mailbox_push({.origin = ORG_SW, .content = STATE_ATTRIBUTE_LCD_DARK}, false);
    dim_alive = false;
    vTaskDelete(NULL);
}