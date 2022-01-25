/*
*****************************************************************************************
project name:       ESP32_P2P_Receiver
auth:               Jakob T.
date:               16.01.22
brief:              driver for ON AIR shield (acts as slave using ESP-Now)
version:            V1.1
*****************************************************************************************
*/

#define VERBOSE_DEBUG

#include "esp32now.h"

QueueHandle_t qCMD;
static int cur_cmd = STATE_IDLE;


void setup() {

  Serial.begin(115200);
  DEBUG("\n****************\nIDENTIFIER: SLAVE, ON AIR SHIELD\n****************\n");
  
  init_gpios();
  init_lcd();
  int chk = init_receiver();
  if(chk != 0)
  {
    DEBUG("Could not init esp-now\n");
    return;
  }
  Serial.printf("Device MAC-address: ");
  Serial.println(get_mac());

  qCMD = init_ISRs();
  show_init_screen();
  delay(2000);
  display_mac(get_mac());
  delay(4000);
  clear_lcd();
}


void loop() {

  while(cur_cmd == STATE_NO_STATE)
  {
    xQueueReceive(qCMD, &cur_cmd, 1);
  }
  handle_cmd(cur_cmd);
  cur_cmd = STATE_NO_STATE;
}