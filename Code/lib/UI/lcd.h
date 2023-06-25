#ifndef _LCD_H_
#define _LCD_H_

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

/// @brief String macros for messages visible on the
/// LCD
/// @note Macros prefixed with TXT_SUB always appear
/// on the second row of the display
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

/// @brief Custom child class of LiquidCrystal_I2C
class LCD: public LiquidCrystal_I2C
{
    // inherit constructor
    using LiquidCrystal_I2C::LiquidCrystal_I2C;

public:

    /// @brief Combo wrapper for I2C and LCD initializers
    void init_all(void);

    /// @brief Init sequence for LCD
    /// @note Writes content of TXT_INIT on the 1. row
    /// and TXT_INIT_SUB on the second one
    void show_init_screen(void);

    /// @brief UI parser for internal states, prints
    /// associated text to LCD
    /// @note Due to strong coupling, the LED array
    /// (red LEDs behind "ON AIR"-font) are controlled
    /// inside of this function
    void display_state(int state);

    /// @brief Fast clear for LCD, prints whitespaces
    void clear(void);

    /// @brief Remove backlight from LCD
    void dim(void);
};

/// @brief Exposure of singelton to rest of hardware module
extern LCD lcd;

#endif