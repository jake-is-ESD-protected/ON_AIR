/*
*****************************************************************************************
header name:        hardware.h
auth:               Jakob T.
date:               15.11.21
brief:              pin definitions and board-specific info
version:            V1.1
*****************************************************************************************
*/

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include <Arduino.h>
#include "SFE_MicroOLED.h"
#include "Wire.h"

#ifdef VERBOSE_DEBUG
  #define DEBUG(msg) {Serial.print(msg);}
#else
  #define DEBUG(msg)
#endif

#define PUSH_PIN  16
#define LED_PIN   19

#define RESET   -1
#define DC      1

void init_gpios(void);


void init_oled(void);


void show_init_screen(void);
#endif