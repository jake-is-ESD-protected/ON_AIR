#include "lcd.h"
#include "states.h"
#include "hardware.h"

LCD lcd(0x27, 16, 2);

void LCD::init_all(void)
{
    Wire.begin();
    Wire.setClock(10000);
    this->init();
    this->backlight();
}


void LCD::display_mac(String mac)
{
    this->setCursor(0, 1);
    this->println(mac);
}


void LCD::show_init_screen(void)
{
    this->setCursor(0, 0);
    this->printf(TXT_INIT);
}


void LCD::display_state(int state)
{
    clear();
    this->backlight();
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


void LCD::clear(void)
{
    // pseudo-clear is faster
    this->setCursor(0, 0);
    this->printf("                ");
    this->setCursor(0, 1);
    this->printf("                ");
    this->setCursor(0, 0);
}


void LCD::dim(void)
{
    this->noBacklight();
}