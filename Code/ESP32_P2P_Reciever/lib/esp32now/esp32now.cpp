/*
*****************************************************************************************
source name:        esp32now.cpp
auth:               Jakob T.
date:               16.01.22
brief:              abstraction for esp-now related functions
version:            V1.1
*****************************************************************************************
*/

#include "esp32now.h"

extern QueueHandle_t qCMD;

// peer address of master ESP
uint8_t dest_addr[] = {0xC8, 0xC9, 0xA3, 0xD2, 0x31, 0x00};

// info object
esp_now_peer_info_t peerInfo;


// on-send callback
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  handle_ESPnow_output(&status);
}


// on-recieve callback
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  // assume cmd is one byte long always
  uint8_t cmd = *incomingData;
  Serial.printf("cmd callback from espnow: %d\n", cmd);
  xQueueSend(qCMD, &cmd, 1);
}


// init all required ESP-NOW stats for this device
int init_receiver(void)
{
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return -1;
    }

    // Register sender-callback
    esp_now_register_send_cb(OnDataSent);
    
    // Register peer
    memcpy(peerInfo.peer_addr, dest_addr, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;

    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return -1;
    }

    // Register reciever-callback
    esp_now_register_recv_cb(OnDataRecv);
    return 0;
}


// simple wrapper for transmission abstraction
esp_err_t esp_send(uint8_t data)
{
  return esp_now_send(dest_addr, (uint8_t *) &data, sizeof(data));
}


// obtain MAC address of device
String get_mac(void)
{
  return WiFi.macAddress();
}
