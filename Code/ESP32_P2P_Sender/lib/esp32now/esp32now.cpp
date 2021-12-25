#include "esp32now.h"

// cmd-buffer
char inc_cmd[5];

// info object
esp_now_peer_info_t peerInfo;


// on-send callback
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  handle_output(&status);
}


// on-recieve callback
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(inc_cmd, incomingData, sizeof(inc_cmd));
  handle_input(inc_cmd, len);
}


int init_sender(void)
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
