#include "tasks.h"
#include "core.h"
#include "mywebserver.h"
#include "hardware.h"

TaskHandle_t tLoop = NULL;

void mainLoopTask(void* param){
    while(1){
        handle_cmd(task_notify_take(portMAX_DELAY));
    }
}


void time_led_task(void* param){
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
    Serial.printf("[DEBUG]\tStarting ring-timeout\r\n");
    while(tim_alive && (i < RING_AMOUNT)){
        vTaskDelay((RING_TIME / RING_AMOUNT) / portTICK_PERIOD_MS);
        i++;
    }
    if(!tim_alive){
        // process forced to exit by outside means
        led_alive = false;
        bell = false;
        Serial.printf("\r\n[DEBUG]\ttimeout interrupted\r\n");
        vTaskDelete(NULL);
    }
    else{
        // process tasked to die after time is up
        Serial.printf("\r\n[DEBUG]\ttimeout quitting\r\n");
        led_alive = false;
        bell = false;
        
        cmd_t c = {
            .origin = ORG_SW,
            .content = STATE_NO_RESPONSE
        };
        task_notify(tLoop, c, false);
        vTaskDelay(DOUBLE_MSG_DELAY * 2 / portTICK_PERIOD_MS);
        c = last_cmd;
        task_notify(tLoop, c, false);
        tim_alive = false;
        vTaskDelete(NULL);
    }
}


void blink_led_task(void* param){
    while(led_alive){
        for(int i = 0; i < 3; i++){
            led_on();
            vTaskDelay(100 / portTICK_PERIOD_MS);
            led_off();
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}


void dim_lcd_task(void* param){
    // check periodically if dimmer is not cancelled from outside       
    uint8_t i = 0;
    while(dim_alive && (i < POWER_SAVE_TIME_SPLIT)){
        vTaskDelay(((POWER_SAVE_TIME / POWER_SAVE_TIME_SPLIT) / portTICK_PERIOD_MS));
        i++;
    }
    if(!dim_alive){
        vTaskDelete(NULL);
    }
    else{
        cmd_t c = {
            .origin = ORG_SW, 
            .content = STATE_ATTRIBUTE_LCD_DARK
            };
        task_notify(tLoop, c, false);
        dim_alive = false;
        vTaskDelete(NULL);
    }
}

void task_notify(TaskHandle_t t, cmd_t c, bool from_isr){
    if(from_isr){
        BaseType_t dummy = pdFALSE;
        xTaskNotifyFromISR(t, *(uint32_t*)&c, eSetValueWithOverwrite, &dummy);
    }
    else{
        xTaskNotify(t, *(uint32_t*)&c, eSetValueWithOverwrite);
    }
}

cmd_t task_notify_take(TickType_t ticks_to_wait){
    uint32_t c = ulTaskNotifyTake(pdTRUE, ticks_to_wait);
    return *(cmd_t*)&c;
}