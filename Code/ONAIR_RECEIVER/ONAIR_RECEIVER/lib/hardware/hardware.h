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
#include "literals.h"
#include "mailbox.h"


// pins
#define BUTTON_LED_PIN            23
#define LED_ARRAY                 18
#define BUTTON_IN_PIN             19

// digital debounce
#define IR_DEBOUNCE_TIME          5000

// timings                 
#define RING_TIME                 10000
#define RING_AMOUNT               (RING_TIME / 2000)
#define POWER_SAVE_TIME           10000
#define POWER_SAVE_TIME_SPLIT     (POWER_SAVE_TIME / 1000)
#define DOUBLE_MSG_DELAY          2000


// interrupt service routine for button (bell)
void IRAM_ATTR BUT_ISR(void);


// init ISR and queue
void init_ISRs(void);


// init all GPIOs as OUTPUT/INPUT
void init_gpios(void);


// turn on LED-array
void led_on(void);


// turn off LED-array
void led_off(void);


// blink button LED by fixed amount
void led_blink(void);

#endif