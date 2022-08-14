/*
*****************************************************************************************
project name:       ESP32_P2P_Receiver
auth:               Jakob T.
date:               16.01.22
brief:              driver for ON AIR shield (acts as slave using ESP-Now)
version:            V1.2
*****************************************************************************************
*/

#define VERBOSE_DEBUG


#include "mailbox.h"
#include "lcd.h"
#include "tasks.h"
#include "states.h"
#include "hardware.h"
#include "webserver.h"


void setup() {

  Serial.begin(115200);
  Serial.printf("\r\n****************\r\nIDENTIFIER: SLAVE, ON AIR SHIELD\r\n****************\r\n");
  
  init_gpios();
  lcd.init_all();

  xTaskCreate(webserverTask,
            "webserver hosting",
            4096,
            NULL,
            1,
            &tWebserver);  

  lcd.show_init_screen();
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  lcd.clear();

  cmd_t c = {
    .origin = ORG_SW,
    .content = STATE_IDLE
  };

  init_ISRs();

  xTaskCreate(mainLoopTask,
              "main driver loop",
              4096,
              NULL,
              1,
              &tLoop);          

  delay(200);            

  mbox.push(c, false);
  mbox.notify(tLoop, false);
}


void loop() {
  // unused
}