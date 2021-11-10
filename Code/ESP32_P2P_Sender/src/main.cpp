/*
*****************************************************************************************
project name:       ESP32_P2P_Sender
auth:               Jakob T.
date:               10.11.21
brief:              driver for ON AIR sign itself (acts as slave usinf ESP-Now)
version:            V1.0
*****************************************************************************************
*/

#include <Arduino.h>
#include "WiFi.h"
#include "esp_now.h"
#include "literals.h"

#define PUSH_PIN  16
#define LED_PIN   19
 
// this is the mac-address of the receiver, in this case the sign outside
uint8_t dest_addr[] = {0x24, 0x6F, 0x28, 0x7B, 0x7C, 0x78};
char inc_cmd[5];
bool bell_ringing = false;
int i = 0;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  memcpy(inc_cmd, incomingData, sizeof(inc_cmd));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print(inc_cmd);
  if(!strcmp(inc_cmd, ID_BELL_INT))
  {
    bell_ringing = true;
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
  }
  else
  {
    bell_ringing = false;
  }
}
 
void setup() {

  Serial.begin(115200);
    Serial.printf("****************\nIDENTIFIER: MASTER, ON AIR CMD\n****************\n");

  pinMode(PUSH_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register sender-callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, dest_addr, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register reciever-callback
  esp_now_register_recv_cb(OnDataRecv);

}
 
void loop() {

  Serial.print("Waiting for push...\n");
  while(digitalRead(PUSH_PIN) == HIGH);
  while(digitalRead(PUSH_PIN) == LOW); // digital debouncer

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(dest_addr, (uint8_t *) cmd[i], sizeof(cmd[i]));
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