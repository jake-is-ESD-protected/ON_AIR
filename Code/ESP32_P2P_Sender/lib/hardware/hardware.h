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

// debug helper
#include <Arduino.h>
#include "SFE_MicroOLED.h"
#include "Wire.h"
#include "bitmaps.h"

#ifdef VERBOSE_DEBUG
  #define DEBUG(msg) {Serial.print(msg);}
#else
  #define DEBUG(msg)
#endif

// pins
#define PUSH_PIN  33
#define RE_DT     32
#define RE_CLK    35
#define LED_RED   16
#define LED_GREEN 17
#define LED_BLUE  18

// command IDs
#define LEFT      100
#define RIGHT     101
#define PUSH      102
#define BELL_INT  103
#define RESET   -1
#define DC      1

#define STATE_IDLE  0
#define STATE_WOEX  1
#define STATE_MEET  2
#define STATE_RECO  3
#define STATE_WAIT  4
#define STATE_WELC  5
#define STATE_BELL  10
#define STATE_ERROR 127
#define MAX_STATE   5

// digital debounce
#define IR_DEBOUNCE_TIME  150

void IRAM_ATTR RE1_ISR();


void IRAM_ATTR RE2_ISR();


void IRAM_ATTR BUT_ISR();


QueueHandle_t init_ISRs(void);


void init_gpios(void);


void init_oled(void);


void show_init_screen(void);


void clear_oled(void);


void drawbmp(int state, bool invert);


#endif