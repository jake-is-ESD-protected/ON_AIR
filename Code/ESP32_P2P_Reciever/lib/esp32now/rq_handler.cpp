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

static uint8_t last_cmd = STATE_IDLE;

void handle_cmd(uint8_t inc_cmd)
{
    TaskHandle_t tTim = NULL;
    TaskHandle_t tLED = NULL;
    TaskHandle_t tDim = NULL;

    /*  
        button has been pressed:
        - proceed to send message to master
        - blink LED for RING_TIME ms
    */
    if(inc_cmd == STATE_BELL && !tim_alive)
    {
        esp_err_t err = esp_send((uint8_t)BELL_INT);
        if(err != ESP_OK)
        {
            Serial.printf("failed to send data: %d", inc_cmd);
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
    if(inc_cmd != STATE_NO_RESPONSE && inc_cmd != STATE_BELL)
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
    if(inc_cmd == STATE_IDLE && !dim_alive)
    {
        xTaskCreate(dim_lcd_task,
                    "go into power saving mode",
                    1024,
                    NULL,
                    1,
                    &tDim);
    }

    /*  
        any action taken while device is entering dim-mode:
        - delete dim-timer
    */
    if(inc_cmd != STATE_IDLE && dim_alive)
    {
        dim_alive = false;
        
        vTaskDelete(tDim);
        Serial.printf("killed dim-task\n");
    }

    lcd_display_state((uint8_t)inc_cmd);

    if(inc_cmd != STATE_NO_RESPONSE && inc_cmd != STATE_BELL)
    {   // skip any instances of the no response state or
        // bell state since they should not be returned to
        last_cmd = inc_cmd;
    }

    Serial.printf("tim-task: %d, led-task: %d, dim-task: %d\n", tim_alive, led_alive, dim_alive);
}


void handle_ESPnow_output(esp_now_send_status_t* status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
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
    led_off();
    led_alive = false;
    int cmd = STATE_NO_RESPONSE;
    xQueueSend(qCMD, &cmd, 1);
    vTaskDelay(DOUBLE_MSG_DELAY * 2 / portTICK_PERIOD_MS);
    cmd = last_cmd;
    xQueueSend(qCMD, &cmd, 1);
    tim_alive = false;
    vTaskDelete(NULL);
}


void toggle_led_task(void* param)
{
    while(1)
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
}


void dim_lcd_task(void* param)  // TASK NOT GETTING KILLED!!!
{
    dim_alive = true;
    vTaskDelay(POWER_SAVE_TIME / portTICK_PERIOD_MS);
    dim_lcd();
    dim_alive = false;
    vTaskDelete(NULL);
}