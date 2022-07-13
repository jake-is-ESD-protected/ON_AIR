#include "mainloop.h"
#include "esp32now.h"
#include "mailbox.h"
#include "rq_handler.h"

TaskHandle_t tLoop = NULL;

void mainloop(void* param)
{
  while(1)
  {
    mbox.wait();
    while(mbox.data_avail())
    {
      handle_cmd(mbox.pop(false));
    }
  }
}