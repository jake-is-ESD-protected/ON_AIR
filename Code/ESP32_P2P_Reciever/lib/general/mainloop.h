#ifndef _MAINLOOP_H_
#define _MAINLOOP_H_

#include "Arduino.h"

void mainloop(void* param);

void webserverTask(void* param);

void wsClientHandler(void* param);

extern TaskHandle_t tLoop;
extern TaskHandle_t tWebserver;
extern TaskHandle_t tClientHandler;

#endif