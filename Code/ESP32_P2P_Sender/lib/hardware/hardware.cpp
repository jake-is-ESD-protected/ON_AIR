/*
*****************************************************************************************
source name:        hardware.cpp
auth:               Jakob T.
date:               16.01.22
brief:              hardware access
version:            V1.2
IMPORTANT:          THIS AND ONLY THIS FILE HAS PERMISSION TO WRITE TO HARDWARE
                    This includes: OLED, Serial, LED
*****************************************************************************************
*/

#include "hardware.h"


MicroOLED oled(-1, 1);
extern portMUX_TYPE mux;
extern QueueHandle_t qCMD;
static unsigned long last_interrupt_time_RE1 = 0;
static unsigned long last_interrupt_time_RE2 = 0;
static unsigned long last_interrupt_time_BUT = 0;


void IRAM_ATTR RE1_ISR() 
{
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time_RE1 > IR_DEBOUNCE_TIME)
    {
        if(digitalRead(RE_CLK) && !(digitalRead(RE_DT) == digitalRead(RE_CLK)))
        {
            cmd_t c = {
                .origin = ORG_HW,
                .content = LEFT
            };
            mailbox_push(c, true);
        }
    }
    last_interrupt_time_RE1 = interrupt_time;
}


void IRAM_ATTR RE2_ISR() 
{
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time_RE2 > IR_DEBOUNCE_TIME)
    {
        if(digitalRead(RE_DT) && !(digitalRead(RE_DT) == digitalRead(RE_CLK)))
        {
            cmd_t c = {
                .origin = ORG_HW,
                .content = RIGHT
            };
            mailbox_push(c, true);
        }
    }
    last_interrupt_time_RE2 = interrupt_time;    
}


void IRAM_ATTR BUT_ISR() 
{
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time_BUT > IR_DEBOUNCE_TIME)
    {
        cmd_t c = {
            .origin = ORG_HW,
            .content = PUSH
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
        return true;
    }
}


QueueHandle_t mailbox_create(uint8_t size)
{
    return xQueueCreate(size, sizeof(cmd_t));
}


QueueHandle_t init_ISRs(void)
{
    attachInterrupt(RE_DT, RE1_ISR, FALLING);
    attachInterrupt(RE_CLK, RE2_ISR, FALLING);
    attachInterrupt(PUSH_PIN, BUT_ISR, FALLING);

    vPortCPUInitializeMutex(&mux);

    qCMD = mailbox_create(30);
    return qCMD;
}


void init_gpios(void)
{
    pinMode(PUSH_PIN, INPUT);
    pinMode(RE_DT, INPUT);
    pinMode(RE_CLK, INPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
}


void init_oled(void)
{
    Wire.begin();
    oled.begin();
    oled.clear(ALL);
    oled.clear(PAGE);
}


void show_init_screen(void)
{
    oled.setCursor(8, 14);
    oled.setFontType(1);
    oled.printf("ON AIR");
    oled.display();
}


void clear_oled(void)
{
    oled.clear(PAGE);
}


void drawbmp(int state, bool invert)
{
    switch(state)
    {
        case STATE_IDLE:
            oled.drawBitmap((uint8_t*)bmp_zzz);
            break;
        case STATE_WOEX:
            oled.drawBitmap((uint8_t*)bmp_work);
            break;
        case STATE_MEET:
            oled.drawBitmap((uint8_t*)bmp_meeting);
            break;
        case STATE_RECO:
            oled.drawBitmap((uint8_t*)bmp_microphone);
            break;
        case STATE_WAIT:
            oled.drawBitmap((uint8_t*)bmp_clock);
            break;
        case STATE_WELC:
            oled.drawBitmap((uint8_t*)bmp_door);
            break;
        case STATE_BELL:
            oled.drawBitmap((uint8_t*)bmp_bell);
            break;
        default:
            oled.setCursor(8, 14);
            oled.setFontType(1);
            oled.printf("???");            
    }
    oled.invert(invert);
    oled.display();
}