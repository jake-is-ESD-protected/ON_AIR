#include "hardware.h"

MicroOLED oled(-1, 1);

void init_gpios(void)
{
    pinMode(PUSH_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
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
    //oled.drawBitmap((uint8_t*)bmp_bell);
    oled.display();
}