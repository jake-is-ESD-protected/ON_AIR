#ifndef _LCD_H_
#define _LCD_H_

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>


#define TXT_INIT                "---[ ON AIR ]---"
#define TXT_INIT_SUB            "V 1.1: Webserver"
#define TXT_IDLE                "Hi, I'm [ONAIR]!"
#define TXT_WORKING_EXCUSE      "I'm working."
#define TXT_MEETING_EXCUSE      "In a meeting"
#define TXT_RECORDING_EXCUSE    "I'm recording!"
#define TXT_WAIT_EXCUSE         "Give me a sec"
#define TXT_WELCOME             "Come in!"
#define TXT_RINGING             "It's ringing..."
#define TXT_NO_RESPONSE         "I'm away."
#define TXT_UNKNOWN             "error: unknown"

#define TXT_SUB_SOON            "Just a minute..."
#define TXT_SUB_LATER           "Later please..."
#define TXT_SUB_AWAY            "Sorry!"
#define TXT_SUB_IDLE            "Call Jakob ---->"


class LCD: public LiquidCrystal_I2C
{
    // inherit constructor
    using LiquidCrystal_I2C::LiquidCrystal_I2C;

public:

    // init the LCD-object
    void init_all(void);


    // init sequence for LCD
    void show_init_screen(void);


    // display current state (same as command)
    void display_state(int state);


    // faster than internal clear, prints whitespaces
    void clear(void);


    // remove backlight from LCD
    void dim(void);
};

// singleton reference:
extern LCD lcd;

#endif