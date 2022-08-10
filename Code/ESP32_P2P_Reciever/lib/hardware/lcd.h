#ifndef _LCD_H_
#define _LCD_H_

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>


#define TXT_INIT                "---[ ON AIR ]---"
#define TXT_INIT_SUB            "V 1.1: Webserver"
#define TXT_IDLE                "Hi, I'm [ONAIR]!"
#define TXT_WORKING_EXCUSE      "Arbeite gerade."
#define TXT_MEETING_EXCUSE      "In einem Meeting"
#define TXT_RECORDING_EXCUSE    "Nehme was auf!"
#define TXT_WAIT_EXCUSE         "Bitte warte kurz"
#define TXT_WELCOME             "Komm doch rein!"
#define TXT_RINGING             "Es klingelt..."
#define TXT_NO_RESPONSE         "Bin leider weg."
#define TXT_UNKNOWN             "error: unknown"

#define TXT_SUB_SOON            "bin gleich da"
#define TXT_SUB_LATER           "später bitte..."
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


    // display MAC-adress on screen
    void display_mac(String mac);


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