#ifndef _MAINLOOP_H_
#define _MAINLOOP_H_

#include "Arduino.h"

void mainloop(void* param);

extern TaskHandle_t tLoop;

#endif