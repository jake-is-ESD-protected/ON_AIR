#include "lcdisplay.h"
#include "states.h"
#include "hardware.h"

LCDisplay lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void LCDisplay::init_all(void)
{
    pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
    this->begin(16, 2);
    this->bright();
}


void LCDisplay::show_init_screen(void)
{
    this->setCursor(0, 0);
    this->printf(TXT_INIT);
    this->setCursor(0, 1);
    this->printf(TXT_INIT_SUB);
}


void LCDisplay::display_state(int state)
{
    clear();
    this->bright();
    switch(state)
    {
        case STATE_IDLE:
            digitalWrite(LED_ARRAY, LOW);
            this->printf(TXT_IDLE);
            this->setCursor(0, 1);
            this->printf(TXT_SUB_IDLE);
            break;
        case STATE_WOEX:
            digitalWrite(LED_ARRAY, HIGH);
            this->printf(TXT_WORKING_EXCUSE);
            break;
        case STATE_MEET:
            digitalWrite(LED_ARRAY, HIGH);
            this->printf(TXT_MEETING_EXCUSE);
            break;
        case STATE_RECO:
            digitalWrite(LED_ARRAY, HIGH);
            this->printf(TXT_RECORDING_EXCUSE);
            break;
        case STATE_WAIT:
            digitalWrite(LED_ARRAY, HIGH);
            this->printf(TXT_WAIT_EXCUSE);
            break;
        case STATE_WELC:
            digitalWrite(LED_ARRAY, LOW);
            this->printf(TXT_WELCOME);
            break;
        case STATE_BELL:
            this->printf(TXT_RINGING);
            break;
        case STATE_NO_RESPONSE:
            this->printf(TXT_NO_RESPONSE);
            vTaskDelay(DOUBLE_MSG_DELAY / portTICK_PERIOD_MS);
            this->setCursor(0, 1);
            this->printf(TXT_SUB_AWAY);
            break;
        case STATE_ATTRIBUTE_LCD_DARK:
            dim();
            break;
        case STATE_ATTRIBUTE_BL_ON:
            digitalWrite(BUTTON_LED_PIN, HIGH);
            break;
        case STATE_ATTRIBUTE_BL_OFF:
            digitalWrite(BUTTON_LED_PIN, LOW);
            break;    
        case STATE_ATTRIBUTE_LA_ON:
            digitalWrite(LED_ARRAY, HIGH);
            break;
        case STATE_ATTRIBUTE_LA_OFF:
            digitalWrite(LED_ARRAY, LOW);
            break;                
        default:
            this->printf(TXT_UNKNOWN);
    }
}


void LCDisplay::clear(void)
{
    // pseudo-clear is faster
    this->setCursor(0, 0);
    this->printf("                ");
    this->setCursor(0, 1);
    this->printf("                ");
    this->setCursor(0, 0);
}


void LCDisplay::bright(void)
{
    digitalWrite(LCD_BACKLIGHT_PIN, HIGH);
}


void LCDisplay::dim(void)
{
    digitalWrite(LCD_BACKLIGHT_PIN, LOW);
}