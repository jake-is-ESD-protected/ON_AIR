#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include "WiFi.h"


#define SSID                "Klus4iot"
#define PWD                 "222Klus4Satt1"
#define WEBSERVER_TIMEOUT   2000

#define BUFLEN              30

class webserver
{
public:
    //delete this
    String idle_state = "off";
    String woex_state = "off";
    
    webserver();

    void init();

    void printIP();

    void run();


private:
    WiFiServer server;

    uint32_t cur_time = 0;
    uint32_t prev_time = 0; 
    uint32_t timeout = WEBSERVER_TIMEOUT;

    String header;
};


extern webserver ws;

#endif