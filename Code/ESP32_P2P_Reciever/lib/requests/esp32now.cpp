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
#include "esp_now.h"
#include "WiFi.h"
#include "rq_handler.h"
#include "mailbox.h"
#include "states.h"

extern QueueHandle_t qCMD;

// peer address of master ESP
uint8_t dest_addr[] = {0x94, 0xB9, 0x7E, 0xD4, 0xD9, 0xF0};

// info object
esp_now_peer_info_t peerInfo;


// on-send callback
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\tLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


// on-recieve callback
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  // assume cmd is one byte long always
  cmd_t cmd = {
    .origin = ORG_NOW,
    .content = *incomingData
  };
  mbox.push(cmd, false);
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
