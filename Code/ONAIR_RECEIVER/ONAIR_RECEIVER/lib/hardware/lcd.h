#ifndef _LCD_H_
#define _LCD_H_

#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include "Wire.h"

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