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
#include "mainloop.h"
#include "debug_print.h"
#include "states.h"
#include "hardware.h"
#include "esp32now.h"


void setup() {

  Serial.begin(115200);
  DEBUG("\r\n****************\r\nIDENTIFIER: SLAVE, ON AIR SHIELD\r\n****************\r\n");
  
  init_gpios();
  lcd.init_all();

  int chk = init_receiver();
  if(chk != 0)
  {
    DEBUG("Could not init esp-now\r\n");
    return;
  }
  Serial.printf("Device MAC-address: ");
  Serial.println(get_mac());

  lcd.show_init_screen();
  delay(2000);
  lcd.display_mac(get_mac());
  delay(4000);
  lcd.clear();

  cmd_t c = {
    .origin = ORG_SW,
    .content = STATE_IDLE
  };

  init_ISRs();

  xTaskCreate(mainloop,
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