#ifndef _ESP32NOW_H_
#define _ESP32NOW_H_

#include "esp_now.h"
#include "rq_handler.h"
#include "WiFi.h"


int init_receiver(void);


esp_err_t esp_send(uint8_t data);


String get_mac(void);


#endif