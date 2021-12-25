/*
*****************************************************************************************
project name:       ESP32_P2P_Sender
auth:               Jakob T.
date:               10.11.21
brief:              driver for ON AIR controler (acts as master using ESP-Now)
version:            V1.0
*****************************************************************************************
*/

#define VERBOSE_DEBUG

#include "esp32now.h"

int i = 0;

bool init_clean = true;


void setup() {

  Serial.begin(115200);
  DEBUG("****************\nIDENTIFIER: MASTER, ON AIR CMD\n****************\n");

  init_gpios();
  init_oled();
  int chk = init_sender();
  if(chk != 0)
  {
    DEBUG("Could not init esp-now\n");
    init_clean = false;
    return;
  }

  show_init_screen();
}
 
void loop() {

  if(init_clean == false)
  {
    DEBUG("init not clean, abort.\n");
    while(1);
  }

  DEBUG("Waiting for push...\n");
  while(digitalRead(PUSH_PIN) == HIGH);
  while(digitalRead(PUSH_PIN) == LOW); // digital debouncer

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send((uint8_t*)dest_addr, (uint8_t *) cmd[i], sizeof(cmd[i]));
  Serial.printf("Sending %s ...", cmd[i]);
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  i++;
  if(i == N_CMDS)
  {
    i = 0;
  }
}