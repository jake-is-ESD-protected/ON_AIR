#ifndef _RQ_HANDLER_H_
#define _RQ_HANDLER_H_

#include "esp_now.h"
#include "WiFi.h"
#include "hardware.h"


void handle_ESPnow_input(char* cmd, int len);


void handle_ESPnow_output(esp_now_send_status_t* status);

#endif