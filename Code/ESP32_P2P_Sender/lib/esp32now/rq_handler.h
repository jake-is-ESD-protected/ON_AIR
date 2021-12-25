#ifndef _RQ_HANDLER_H_
#define _RQ_HANDLER_H_

#include "esp_now.h"
#include "WiFi.h"
#include "literals.h"
#include "hardware.h"
#include "bitmaps.h"

void handle_input(char* cmd, int len);

void handle_output(esp_now_send_status_t* status);

#endif