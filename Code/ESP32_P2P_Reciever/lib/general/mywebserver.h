#ifndef _MYWEBSERVER_H_
#define _MYWEBSERVER_H_

#include <WiFi.h>
#include <WebServer.h>


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

    void run();

    void handle(uint8_t state);

    String SendHTML(uint8_t state);

    WebServer server;

    
private:
    uint32_t cur_time = 0;
    uint32_t prev_time = 0; 
    uint32_t timeout = WEBSERVER_TIMEOUT;

    String header;
};

void handle_onConnect();

void handle_idle();

void handle_woex();

void handle_meet();

void handle_reco();

void handle_wait();

void handle_welc();

void handle_NotFound();

void handle_getState();

String SendHTML(uint8_t state);


extern webserver ws;

#endif