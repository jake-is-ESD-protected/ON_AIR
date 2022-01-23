#ifndef _RQ_HANDLER_H_
#define _RQ_HANDLER_H_

#include "hardware.h"
#include "esp32now.h"


void handle_ESPnow_output(esp_now_send_status_t* status);


int handle_cmd(int inc_cmd);

#endif