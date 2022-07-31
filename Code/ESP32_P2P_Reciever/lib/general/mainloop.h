#ifndef _MAINLOOP_H_
#define _MAINLOOP_H_

#include "Arduino.h"

void mainloop(void* param);

void webserverTask(void* param);

void _webserverListener(void* param);

extern TaskHandle_t tLoop;
extern TaskHandle_t tWebserver;

#endif