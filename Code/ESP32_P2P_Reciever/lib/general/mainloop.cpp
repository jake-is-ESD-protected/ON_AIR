#include "mainloop.h"
#include "esp32now.h"

TaskHandle_t tLoop = NULL;

void mainloop(void* param)
{
  while(1)
  {
    handle_cmd(mbox.pop(true));
  }
}