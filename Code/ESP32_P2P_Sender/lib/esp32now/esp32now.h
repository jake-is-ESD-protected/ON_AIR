#ifndef _ESP32NOW_H_
#define _ESP32NOW_H_

#include "rq_handler.h"

const char dest_addr[] = {0x24, 0x6F, 0x28, 0x7B, 0x7C, 0x78};

int init_sender(void);

#endif