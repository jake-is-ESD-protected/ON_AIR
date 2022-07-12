#ifndef _POLLING_H_
#define _POLLING_H_

#include <LiquidCrystal_I2C.h>

#include "literals.h"

#define LED_ARRAY   18
#define BUTTON      19
#define BUTTON_LED  23

#define BT_BUF_SIZE 5

String msg_handler(LiquidCrystal_I2C* plcd, String msg);

#endif