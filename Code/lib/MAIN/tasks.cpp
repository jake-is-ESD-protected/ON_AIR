#include "tasks.h"
#include "core.h"
#include "mywebserver.h"
#include "hardware.h"

void mainLoopTask(void* param){
    while(1){
        handle_cmd(task_notify_take(portMAX_DELAY));
    }
}


void time_led_task(void* param){
    set_flag(f_timer_alive, true);
    set_flag(f_led_alive, true);
    TaskHandle_t* p_t_LED = get_task_handle(t_led);
    xTaskCreate(blink_led_task,
                    "blink green LED",
                    1024,
                    NULL,
                    1,
                    p_t_LED);

    // check periodically if timer is not cancelled from outside       
    uint8_t i = 0;
    Serial.printf("[DEBUG]\tStarting ring-timeout\r\n");
    while(get_flag(f_timer_alive) && (i < RING_AMOUNT)){
        vTaskDelay((RING_TIME / RING_AMOUNT) / portTICK_PERIOD_MS);
        i++;
    }
    if(!get_flag(f_timer_alive)){
        // process forced to exit by outside means
        set_flag(f_led_alive, false);
        set_flag(f_bell_alive, false);
        Serial.printf("\r\n[DEBUG]\ttimeout interrupted\r\n");
        vTaskDelete(NULL);
    }
    else{
        // process tasked to die after time is up
        Serial.printf("\r\n[DEBUG]\ttimeout quitting\r\n");
        set_flag(f_led_alive, false);
        set_flag(f_bell_alive, false);
        
        cmd_t c = {
            .origin = ORG_SW,
            .content = STATE_NO_RESPONSE
        };
        TaskHandle_t* p_t_loop = get_task_handle(t_loop);
        task_notify(*p_t_loop, c, false);
        vTaskDelay(DOUBLE_MSG_DELAY * 2 / portTICK_PERIOD_MS);
        c = last_cmd;
        task_notify(*p_t_loop, c, false);
        set_flag(f_timer_alive, false);
        vTaskDelete(NULL);
    }
}


void blink_led_task(void* param){
    while(get_flag(f_led_alive)){
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
    while(get_flag(f_dimmer_alive) && (i < POWER_SAVE_TIME_SPLIT)){
        vTaskDelay(((POWER_SAVE_TIME / POWER_SAVE_TIME_SPLIT) / portTICK_PERIOD_MS));
        i++;
    }
    if(!get_flag(f_dimmer_alive)){
        vTaskDelete(NULL);
    }
    else{
        cmd_t c = {
            .origin = ORG_SW, 
            .content = STATE_ATTRIBUTE_LCD_DARK
            };
        TaskHandle_t* p_t_loop = get_task_handle(t_loop);
        task_notify(*p_t_loop, c, false);
        set_flag(f_dimmer_alive, false);
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