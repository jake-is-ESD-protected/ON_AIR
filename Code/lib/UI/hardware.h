#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include "Arduino.h"

/// @brief Pin definitions specific to PCB
#define BUTTON_LED_PIN            23
#define LED_ARRAY                 18
#define BUTTON_IN_PIN             19

/// @brief Timing macros for transitional jobs                 
#define RING_TIME                 10000
#define RING_AMOUNT               (RING_TIME / 2000)
#define POWER_SAVE_TIME           10000
#define POWER_SAVE_TIME_SPLIT     (POWER_SAVE_TIME / 1000)
#define DOUBLE_MSG_DELAY          2000
#define IR_DEBOUNCE_TIME          5000

/// @brief Interrupt service routine for button (bell)
/// @note Includes a digital debounce specified in the
/// macro IR_DEBOUNCE_TIME
void IRAM_ATTR BUT_ISR(void);


/// @brief Init interrupt service routine for button
/// @note Uses Arduino's attachInterrupt()
void init_ISRs(void);


/// @brief Init all GPIOs as OUTPUT/INPUT
/// @note GPIOs in question are found as BUTTON_LED_PIN,
/// LED_ARRAY and BUTTON_IN_PIN
void init_gpios(void);


/// @brief Turn on LED-array
/// @note Wrapper for digitalWrite(), has effect on button LED
void led_on(void);


/// @brief Turn off LED-array
/// @note Wrapper for digitalWrite(), has effect on button LED
void led_off(void);


/// @brief Blink button LED by fixed amount
/// @note has effect on LED of button
void led_blink(void);

#endif