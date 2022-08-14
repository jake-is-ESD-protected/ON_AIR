#include "tasks.h"
#include "mailbox.h"
#include "core.h"
#include "mywebserver.h"
#include "hardware.h"

TaskHandle_t tLoop = NULL;
TaskHandle_t tWebserver = NULL;
TaskHandle_t tClientHandler = NULL;

void mainLoopTask(void* param)
{
  while(1)
  {
    mbox.wait();
    while(mbox.data_avail())
    {
      core.handle_cmd(mbox.pop(NON_BLOCKING));
    }
  }
}


void webserverTask(void* param)
{
  ws.init();

  while(1)
  {
    ws.run();
  }
}


void wsClientHandlerTask(void* param)
{
  while(1)
  {
    ws.server.handleClient();
  }
}


void time_led_task(void* param)
{
    core.tim_alive = true;
    core.led_alive = true;
    TaskHandle_t tLED = NULL;
    xTaskCreate(blink_led_task,
                    "blink green LED",
                    1024,
                    NULL,
                    1,
                    &tLED);

    // check periodically if timer is not cancelled from outside       
    uint8_t i = 0;
    while(core.tim_alive && (i < RING_AMOUNT))
    {
        vTaskDelay((RING_TIME / RING_AMOUNT) / portTICK_PERIOD_MS);
        i++;
    }
    if(!core.tim_alive)
    {
        // process forced to exit by outside means
        core.led_alive = false;
        vTaskDelete(NULL);
    }
    else
    {
        // process tasked to die after time is up
        core.led_alive = false;
        mbox.push({.origin = ORG_SW, .content = STATE_ATTRIBUTE_LA_OFF}, false);
        
        cmd_t cmd = {
            .origin = ORG_SW,
            .content = STATE_NO_RESPONSE
        };
        mbox.push(cmd, false);
        mbox.notify(tLoop, false);
        vTaskDelay(DOUBLE_MSG_DELAY * 2 / portTICK_PERIOD_MS);
        cmd = core.last_cmd;
        mbox.push(cmd, false);
        mbox.notify(tLoop, false);
        core.tim_alive = false;
        vTaskDelete(NULL);
    }
}


void blink_led_task(void* param)
{
    while(core.led_alive)
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
    while(core.dim_alive && (i < POWER_SAVE_TIME_SPLIT))
    {
        vTaskDelay(((POWER_SAVE_TIME / POWER_SAVE_TIME_SPLIT) / portTICK_PERIOD_MS));
        i++;
    }
    if(!core.dim_alive)
    {
        vTaskDelete(NULL);
    }
    else
    {
        mbox.push({.origin = ORG_SW, .content = STATE_ATTRIBUTE_LCD_DARK}, false);
        mbox.notify(tLoop, false);
        core.dim_alive = false;
        vTaskDelete(NULL);
    }
}