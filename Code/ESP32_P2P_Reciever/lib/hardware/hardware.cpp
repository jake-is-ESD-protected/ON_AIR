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
extern portMUX_TYPE mux;
extern QueueHandle_t qCMD;
static unsigned long last_interrupt_time_BUT = 0;


void IRAM_ATTR BUT_ISR() 
{
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time_BUT > IR_DEBOUNCE_TIME)
    {
        cmd_t c = {
            .origin = ORG_HW,
            .content = STATE_BELL
        };
        mailbox_push(c, true);
    }
    last_interrupt_time_BUT = interrupt_time;
}


void mailbox_push(cmd_t cmd, bool fromISR)
{
    portENTER_CRITICAL(&mux);
    if(fromISR)
    {
        xQueueSendFromISR(qCMD, &cmd, NULL);
    }
    else
    {
        xQueueSend(qCMD, &cmd, 1);
    }
    Serial.printf("[PUSH] '%d' - command of origin '%d'\r\n", cmd.content, cmd.origin);
    portEXIT_CRITICAL(&mux);
}


cmd_t mailbox_pop(void)
{
    portENTER_CRITICAL(&mux);
    cmd_t cmd = {
        .origin = ORG_SW,
        .content = STATE_NO_STATE
    };
    xQueueReceive(qCMD, &cmd, 1);
    Serial.printf("[POP]  '%d' - command of origin '%d'\r\n", cmd.content, cmd.origin);
    portEXIT_CRITICAL(&mux);
    return cmd;
}


bool mailbox_data_avail(void)
{
    if(uxQueueMessagesWaiting(qCMD) == 0)
    {
        return false;
    }
    else
    {
        //Serial.printf("\tMessages waiting: %d\r\n", uxQueueMessagesWaiting(qCMD));
        return true;
    }
}


QueueHandle_t mailbox_create(uint8_t size)
{
    return xQueueCreate(size, sizeof(cmd_t));
}


QueueHandle_t init_ISRs(void)
{
    attachInterrupt(BUTTON_IN_PIN, BUT_ISR, FALLING);
    vPortCPUInitializeMutex(&mux);

    qCMD = mailbox_create(30);
    return qCMD;
}


void init_gpios(void)
{
    pinMode(BUTTON_IN_PIN, INPUT);
    pinMode(LED_ARRAY, OUTPUT);
    pinMode(BUTTON_LED_PIN, OUTPUT);
}


void led_on(void)
{
    digitalWrite(BUTTON_LED_PIN, HIGH);
}


void led_off(void)
{
    digitalWrite(BUTTON_LED_PIN, LOW);
}


void led_blink(void)
{
    for(int k = 0; k < RING_TIME; k++)
    {
        for(int i = 0; i < 3; i++)
        {
            
            digitalWrite(BUTTON_LED_PIN, HIGH);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            digitalWrite(BUTTON_LED_PIN, LOW);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void init_lcd(void)
{
    Wire.begin();
    lcd.init();
    lcd.backlight();
}


void display_mac(String mac)
{
    lcd.setCursor(0, 1);
    lcd.println(mac);
}


void show_init_screen(void)
{
    lcd.setCursor(0, 0);
    lcd.printf(TXT_INIT);
}


void lcd_display_state(int state)
{
    clear_lcd();
    lcd.backlight();
    switch(state)
    {
        case STATE_IDLE:
            lcd.printf(TXT_IDLE);
            lcd.setCursor(0, 1);
            lcd.printf(TXT_SUB_IDLE);
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
        case STATE_NO_RESPONSE:
            lcd.printf(TXT_NO_RESPONSE);
            vTaskDelay(DOUBLE_MSG_DELAY / portTICK_PERIOD_MS);
            lcd.setCursor(0, 1);
            lcd.printf(TXT_SUB_AWAY);
            break;
        case STATE_ATTRIBUTE_LCD_DARK:
            dim_lcd();
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
            lcd.printf(TXT_UNKNOWN);
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


void dim_lcd(void)
{
    lcd.noBacklight();
}
