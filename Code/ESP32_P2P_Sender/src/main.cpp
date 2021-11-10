#include <Arduino.h>
#include "WiFi.h"
#include <esp_now.h>

uint8_t mac_addr_sender[] = {0xC8, 0xC9, 0xA3, 0xD1, 0x38, 0x40};
 
// this is the mac-address of the receiver, in this case the sign outside
uint8_t dest_addr[] = {0x24, 0x6F, 0x28, 0x7B, 0x7C, 0x78};

char inc_cmd[5];

// this callback is registered further down
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  memcpy(&inc_cmd, incomingData, sizeof(inc_cmd));
  Serial.print("Bytes received: ");
  Serial.println(len);
}
 
void setup() {

  Serial.begin(115200);
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

  // Send message via ESP-NOW
  uint8_t data[] = "dummy data";
  esp_err_t result = esp_now_send(dest_addr, (uint8_t *) data, sizeof(data));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(10000);
}