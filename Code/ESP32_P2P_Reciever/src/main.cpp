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
  clear_lcd();
}


void loop() {

  
  while(cur_cmd == STATE_NO_STATE)
  { // nothing to compute here
    xQueueReceive(qCMD, &cur_cmd, 1);
  }
  handle_cmd(cur_cmd);
  if(cur_cmd == STATE_BELL)
  {
    esp_err_t err = esp_send((uint8_t)BELL_INT);
    if(err != ESP_OK)
    {
      Serial.printf("failed to send data: %d", cur_cmd);
    }
    while(cur_cmd == 0)
    {
      xQueueReceive(qCMD, &cur_cmd, 1);
    }
    
  }
  cur_cmd = STATE_NO_STATE;
  
  // digitalWrite(BUTTON_LED_PIN, LOW);
  // while(digitalRead(BUTTON_IN_PIN) == HIGH);
  // bell_ringing = true;
  
  // uint8_t data[] = ID_BELL_INT;
  // esp_err_t err = esp_now_send(dest_addr, (uint8_t *) data, sizeof(data));
  
  // DEBUG_ONLY(                                   
  // if (err == ESP_OK) {                          
  //   Serial.println("Sent with success");        
  // }                                             
  // else {                                        
  //   Serial.println("Error sending the data");   
  // })

  // lcd.clear();
  // delay(20);
  // lcd.print(TXT_RINGING);
  // while(bell_ringing)
  // {
  //   digitalWrite(BUTTON_LED_PIN, HIGH);
  //   delay(100);
  //   digitalWrite(BUTTON_LED_PIN, LOW);
  //   delay(100);
  //   digitalWrite(BUTTON_LED_PIN, HIGH);
  //   delay(100);
  //   digitalWrite(BUTTON_LED_PIN, LOW);
  //   delay(1000);
  // }
}