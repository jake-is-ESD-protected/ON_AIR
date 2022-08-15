#ifndef _MYWEBSERVER_H_
#define _MYWEBSERVER_H_

#include <WiFi.h>
#include <WebServer.h>
#include <ESPAsyncWebServer.h>


#define SSID                "Klus4iot"
#define PWD                 "222Klus4Satt1"
#define WEBSERVER_TIMEOUT   2000

#define BUFLEN              30

class webserver
{
public:
    
    webserver();

    void init();

    void printIP();

    AsyncWebServer server;

    
private:
    uint32_t cur_time = 0;
    uint32_t prev_time = 0; 
    uint32_t timeout = WEBSERVER_TIMEOUT;
};

#endif