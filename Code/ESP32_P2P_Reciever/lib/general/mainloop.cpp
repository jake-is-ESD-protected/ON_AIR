#include "mainloop.h"
#include "esp32now.h"
#include "mailbox.h"
#include "rq_handler.h"
#include "webserver.h"

TaskHandle_t tLoop = NULL;
TaskHandle_t tWebserver = NULL;

void mainloop(void* param)
{
  while(1)
  {
    mbox.wait();
    while(mbox.data_avail())
    {
      handle_cmd(mbox.pop(NON_BLOCKING));
    }
  }
}


void webserverTask(void* param)
{
  ws.init();

  while(1)
  {
    ws.run();
  }
}