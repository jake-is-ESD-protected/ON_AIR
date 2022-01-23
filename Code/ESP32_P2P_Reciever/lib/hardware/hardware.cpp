/*
*****************************************************************************************
source name:        hardware.cpp
auth:               Jakob T.
date:               16.01.22
brief:              hardware access
version:            V1.2
IMPORTANT:          THIS AND ONLY THIS FILE HAS PERMISSION TO WRITE TO HARDWARE
                    This includes: lcd, Serial, LED
*****************************************************************************************
*/

#include "hardware.h"


LiquidCrystal_I2C lcd(0x27, 16, 2);


extern QueueHandle_t qCMD;
static unsigned long last_interrupt_time_BUT = 0;


void IRAM_ATTR BUT_ISR() 
{
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time_BUT > IR_DEBOUNCE_TIME)
    {
        int c = STATE_BELL;
        xQueueSendFromISR(qCMD, &c, NULL);
    }
    last_interrupt_time_BUT = interrupt_time;
}


QueueHandle_t init_ISRs(void)
{
    attachInterrupt(BUTTON_IN_PIN, BUT_ISR, FALLING);

    qCMD = xQueueCreate(50, sizeof(uint8_t));
    return qCMD;
}


void init_gpios(void)
{
    pinMode(BUTTON_IN_PIN, INPUT);
    pinMode(LED_ARRAY, OUTPUT);
    pinMode(BUTTON_LED_PIN, OUTPUT);
}


void init_lcd(void)
{
    Wire.begin();
    lcd.init();
    lcd.backlight();
}


void show_init_screen(void)
{
    lcd.setCursor(0, 0);
    lcd.printf("---[ ON AIR ]---");
}


void lcd_display_state(int state)
{
    clear_lcd();
    switch(state)
    {
        case STATE_IDLE:
            lcd.printf(TXT_IDLE);
            digitalWrite(LED_ARRAY, LOW);
            break;
        case STATE_WOEX:
            lcd.printf(TXT_WORKING_EXCUSE);
            digitalWrite(LED_ARRAY, HIGH);
            break;
        case STATE_MEET:
            lcd.printf(TXT_MEETING_EXCUSE);
            digitalWrite(LED_ARRAY, HIGH);
            break;
        case STATE_RECO:
            lcd.printf(TXT_RECORDING_EXCUSE);
            digitalWrite(LED_ARRAY, HIGH);
            break;
        case STATE_WAIT:
            lcd.printf(TXT_WAIT_EXCUSE);
            digitalWrite(LED_ARRAY, HIGH);
            break;
        case STATE_WELC:
            lcd.printf(TXT_WELCOME);
            digitalWrite(LED_ARRAY, LOW);
            break;
        case STATE_BELL:
            lcd.printf(TXT_RINGING);
            break;
        default:
            lcd.printf(TXT_UNKNOWN);
            digitalWrite(LED_ARRAY, LOW);
    }
}


void clear_lcd(void)
{
    // pseudo-clear is faster
    lcd.setCursor(0, 0);
    lcd.printf("                ");
    lcd.setCursor(0, 1);
    lcd.printf("                ");
    lcd.setCursor(0, 0);
}
