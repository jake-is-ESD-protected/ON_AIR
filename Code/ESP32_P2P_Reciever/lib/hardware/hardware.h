/*
*****************************************************************************************
header name:        hardware.h
auth:               Jakob T.
date:               16.01.22
brief:              pin definitions and board-specific info
version:            V1.1
*****************************************************************************************
*/

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include "Wire.h"
#include "literals.h"

// debug helper
#ifdef VERBOSE_DEBUG
  #define DEBUG(msg) {Serial.print(msg);}
#else
  #define DEBUG(msg)
#endif

// pins
#define BUTTON_LED_PIN      23
#define LED_ARRAY           18
#define BUTTON_IN_PIN       19

// command IDs
#define BELL_INT            103

// state IDs
#define STATE_NO_STATE      -1
#define STATE_IDLE          0
#define STATE_WOEX          1
#define STATE_MEET          2
#define STATE_RECO          3
#define STATE_WAIT          4
#define STATE_WELC          5
#define STATE_BELL          10
#define STATE_NO_RESPONSE   11
#define STATE_ERROR         127

// digital debounce
#define IR_DEBOUNCE_TIME    400

// timings
#define RING_TIME           20000
#define POWER_SAVE_TIME     20000
#define DOUBLE_MSG_DELAY    2000


void IRAM_ATTR BUT_ISR();


QueueHandle_t init_ISRs(void);


void init_gpios(void);


void led_on(void);


void led_off(void);


void init_lcd(void);


void show_init_screen(void);


void display_mac(String mac);


void lcd_display_state(int state);


void clear_lcd(void);


void dim_lcd(void);

#endif