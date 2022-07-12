#include "polling.h"

String msg_handler(LiquidCrystal_I2C* plcd, String msg)
{
  
  if(msg == "LAON")
  {
    digitalWrite(LED_ARRAY, HIGH);
  }
  else if(msg == "LAOF")
  {
    digitalWrite(LED_ARRAY, LOW);
  }
  else if(msg == "WOEX")
  {
    plcd->clear();
    plcd->print(TXT_WORKING_EXCUSE);
  }
  else if(msg == "MEET")
  {
    plcd->clear();
    plcd->print(TXT_MEETING_EXCUSE);
  }
  else if(msg == "RECO")
  {
    plcd->clear();
    plcd->print(TXT_RECORDING_EXCUSE);
  }
  else if(msg == "WAIT")
  {
    plcd->clear();
    plcd->print(TXT_WAIT_EXCUSE);
  }
  else if(msg == "WELC")
  {
    plcd->clear();
    plcd->print(TXT_WELCOME);
  }
  else
  {
    plcd->clear();
    plcd->print(TXT_UNKNOWN);
    plcd->setCursor(0, 1);
    plcd->print(msg + "...");
    return ID_UNKNOWN;
  }
  return msg;
}