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

#include "esp32now.h"

QueueHandle_t qCMD;
portMUX_TYPE mux;

void setup() {

  Serial.begin(115200);
  DEBUG("\r\n****************\r\nIDENTIFIER: SLAVE, ON AIR SHIELD\r\n****************\r\n");
  
  init_gpios();
  init_lcd();
  int chk = init_receiver();
  if(chk != 0)
  {
    DEBUG("Could not init esp-now\r\n");
    return;
  }
  Serial.printf("Device MAC-address: ");
  Serial.println(get_mac());

  show_init_screen();
  qCMD = init_ISRs();
  delay(2000);
  display_mac(get_mac());
  delay(4000);
  clear_lcd();
  cmd_t c = {
    .origin = ORG_SW,
    .content = STATE_IDLE
  };
  mailbox_push(c, false);
}


void loop() {

  while(!mailbox_data_avail())
  {
    // @_@
  }
  handle_cmd(mailbox_pop());
}