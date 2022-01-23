#ifndef _ESP32NOW_H_
#define _ESP32NOW_H_

#include "esp_now.h"
#include "WiFi.h"
#include "rq_handler.h"


int init_sender(void);


esp_err_t esp_send(uint8_t data);


String get_mac(void);

#endif