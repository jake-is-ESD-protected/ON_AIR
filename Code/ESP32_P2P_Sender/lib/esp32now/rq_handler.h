#ifndef _RQ_HANDLER_H_
#define _RQ_HANDLER_H_

#include "hardware.h"
#include "esp32now.h"


void handle_ESPnow_output(esp_now_send_status_t* status);


uint8_t handle_cmd(cmd_t inc_cmd);

#endif