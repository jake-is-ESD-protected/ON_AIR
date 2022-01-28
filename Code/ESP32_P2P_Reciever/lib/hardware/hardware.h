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
#define BUTTON_LED_PIN            23
#define LED_ARRAY                 18
#define BUTTON_IN_PIN             19

// command IDs      
#define BELL_INT                  103

// persistant states
#define STATE_IDLE                0
#define STATE_WOEX                1
#define STATE_MEET                2
#define STATE_RECO                3
#define STATE_WAIT                4
#define STATE_WELC                5

// transitional states  
#define STATE_BELL                60
#define STATE_NO_RESPONSE         70
#define STATE_NO_STATE            80
#define STATE_ERROR               90

// state attributes
#define STATE_ATTRIBUTE_LCD_DARK  100
#define STATE_ATTRIBUTE_BL_ON     101
#define STATE_ATTRIBUTE_BL_OFF    102
#define STATE_ATTRIBUTE_LA_ON     103
#define STATE_ATTRIBUTE_LA_OFF    104

// digital debounce
#define IR_DEBOUNCE_TIME          400

// timings      
#define RING_TIME                 20000
#define POWER_SAVE_TIME           10000
#define DOUBLE_MSG_DELAY          2000

// command data type
typedef struct {
    uint8_t origin;
    uint8_t content;
} cmd_t;

// command origins
#define ORG_HW  0 // command stems from hardware (interrupt)
#define ORG_SW  1 // command stems from software (task callback)
#define ORG_NOW 2 // command stems from ESP NOW (master device)


// interrupt service routine for button (bell)
void IRAM_ATTR BUT_ISR(void);


// wrapper for xQueueSend which acts as entry point
void mailbox_push(cmd_t cmd, bool fromISR);


// wrapper for xQueueReceive
cmd_t mailbox_pop(void);


//
bool mailbox_data_avail(void);


// wrapper for xQueueCreate
QueueHandle_t mailbox_create(uint8_t size);


// init ISR and queue
QueueHandle_t init_ISRs(void);


// init all GPIOs as OUTPUT/INPUT
void init_gpios(void);


// turn on LED-array
void led_on(void);


// turn off LED-array
void led_off(void);


// init the LCD-object
void init_lcd(void);


// init sequence for LCD
void show_init_screen(void);


// display MAC-adress on screen
void display_mac(String mac);


// display current state (same as command)
void lcd_display_state(int state);


// faster than internal clear, prints whitespaces
void clear_lcd(void);


// remove backlight from LCD
void dim_lcd(void);

#endif