/*
*****************************************************************************************
project name:       ESP32_P2P_Sender
auth:               Jakob T.
date:               10.11.21
brief:              driver for ON AIR controler (acts as master using ESP-Now)
version:            V1.0
*****************************************************************************************
*/

#include "esp32now.h"

QueueHandle_t qCMD;
portMUX_TYPE mux;


void setup() {

  Serial.begin(115200);
  DEBUG("\n****************\nIDENTIFIER: MASTER, ON AIR CMD\n****************\n");

  init_gpios();
  init_oled();
  int chk = init_sender();
  if(chk != 0)
  {
    DEBUG("Could not init esp-now\n");
    return;
  }
  Serial.printf("Device MAC-address: ");
  Serial.println(get_mac());


  show_init_screen();
  delay(2000);
  clear_oled();
  
  cmd_t c = {
    .origin = ORG_SW,
    .content = LEFT
  };

  qCMD = init_ISRs();
  mailbox_push(c, false); // jump to idle screen
}
 

void loop() {
  
  while(!mailbox_data_avail())
  {
    // @_@
  }
  handle_cmd(mailbox_pop());
}