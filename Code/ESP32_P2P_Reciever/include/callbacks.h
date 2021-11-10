#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_

#include "esp_now.h"
#include "WiFi.h"

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

#endif