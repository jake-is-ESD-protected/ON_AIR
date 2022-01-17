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

QueueHandle_t qCMD;
static int cur_cmd = 0;


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

  qCMD = init_ISRs();
  show_init_screen();
  delay(2000);
  clear_oled();
  cur_cmd = LEFT; // jump to idle screen
}
 

void loop() {
  while(cur_cmd == 0)
  {
    xQueueReceiveFromISR(qCMD, &cur_cmd, NULL);
  }
  
  int cur_state = handle_hw_cmd(cur_cmd);

  cur_cmd = 0;

  //DEBUG("Waiting for input...\n");

  //*******************
  // YOU ARE HERE
  //*******************
  //DEBUG("processing command\n");
  
  
  // // Send message via ESP-NOW
  // esp_err_t result = esp_now_send((uint8_t*)dest_addr, (uint8_t *) cmd[i], sizeof(cmd[i]));
  // Serial.printf("Sending %s ...", cmd[i]);
   
  // if (result == ESP_OK) {
  //   Serial.println("Sent with success");
  // }
  // else {
  //   Serial.println("Error sending the data");
  // }
  // i++;
  // if(i == N_CMDS)
  // {
  //   i = 0;
  // }
}