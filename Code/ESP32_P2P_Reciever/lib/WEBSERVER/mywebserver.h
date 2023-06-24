#ifndef _MYWEBSERVER_H_
#define _MYWEBSERVER_H_

#include <WiFi.h>
#include <WebServer.h>
#include <ESPAsyncWebServer.h>


#define SSID                "MagentaWLAN-NZNV"
#define PWD                 "55359820945153582034"
#define WEBSERVER_TIMEOUT   2000

#define BUFLEN              30

class mywebserver
{
public:
    
    mywebserver();

    void init();

    void printIP();

    AsyncWebServer* server;

    
private:
    uint32_t cur_time = 0;
    uint32_t prev_time = 0; 
    uint32_t timeout = WEBSERVER_TIMEOUT;
};

#endif