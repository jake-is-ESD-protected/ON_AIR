#include "mainloop.h"
#include "mailbox.h"
#include "rq_handler.h"
#include "mywebserver.h"

TaskHandle_t tLoop = NULL;
TaskHandle_t tWebserver = NULL;
TaskHandle_t tClientHandler = NULL;

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


void wsClientHandler(void* param)
{
  while(1)
  {
    ws.server.handleClient();
  }
}