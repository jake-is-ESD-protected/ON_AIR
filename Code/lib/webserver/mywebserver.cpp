#include "mywebserver.h"
#include "states.h"
#include "tasks.h"
#include "index.h"
#include "core.h"
#include "passwords.h"

AsyncWebServer* server;


void mywebserver_printIP()
{
    Serial.printf("[SRVR]\tIP address: ");
    Serial.println(WiFi.localIP());
}

void mywebserver_init()
{
    uint32_t cur_time = millis();
    Serial.printf("[SRVR]\tConnecting to %s ", SSID);
    WiFi.begin(SSID, PWD);
    while (WiFi.status() != WL_CONNECTED) {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        Serial.printf(".");
    }
    Serial.printf("\n[SRVR]\tWiFi connected on %d ms after boot.\n", 
                (uint32_t)((millis() - cur_time) / portTICK_PERIOD_MS));
    mywebserver_printIP();

    static AsyncWebServer s(80);
    server = &s;

    // index
    server->on("/", HTTP_POST, [](AsyncWebServerRequest* req){
        Serial.printf("[SRVR]\tIn handle_onConnect\n");
        req->send(200, "text/html", ws_index);
    });


    // idle
    server->on("/idle", HTTP_POST, [&](AsyncWebServerRequest* req){
        Serial.printf("[SRVR]\tIn handle_idle\n");
        handle_cmd({.origin=ORG_WS, .content=STATE_IDLE});
    });

    // woex
    server->on("/woex", HTTP_POST, [&](AsyncWebServerRequest* req){
        Serial.printf("[SRVR]\tIn handle_woex\n");
        handle_cmd({.origin=ORG_WS, .content=STATE_WOEX});
    });

    // meet
    server->on("/meet", HTTP_POST, [&](AsyncWebServerRequest* req){
        Serial.printf("[SRVR]\tIn handle_meet\n");
        handle_cmd({.origin=ORG_WS, .content=STATE_MEET});
    });

    // reco
    server->on("/reco", HTTP_POST, [&](AsyncWebServerRequest* req){
        Serial.printf("[SRVR]\tIn handle_reco\n");
        handle_cmd({.origin=ORG_WS, .content=STATE_RECO});
    });

    // wait
    server->on("/wait", HTTP_POST, [&](AsyncWebServerRequest* req){
        Serial.printf("[SRVR]\tIn handle_wait\n");
        handle_cmd({.origin=ORG_WS, .content=STATE_WAIT});
    });

    // welc
    server->on("/welc", HTTP_POST, [&](AsyncWebServerRequest* req){
        Serial.printf("[SRVR]\tIn handle_welc\n");
        handle_cmd({.origin=ORG_WS, .content=STATE_WELC});
    });

    // getBell
    server->on("/getState", HTTP_GET, [&](AsyncWebServerRequest* req){

        if(bell)
        {
            Serial.printf("[SRVR]\ttrying to send a bell!\n");
            req->send(200, "text/html", "bell");
        }
        else
        {
            switch(last_cmd.content)
            {
                case STATE_IDLE:
                req->send(200, "text/html", "idle");
                break;

                case STATE_WOEX:
                req->send(200, "text/html", "woex");
                break;

                case STATE_MEET:
                req->send(200, "text/html", "meet");
                break;

                case STATE_RECO:
                req->send(200, "text/html", "reco");
                break;

                case STATE_WAIT:
                req->send(200, "text/html", "wait");
                break;

                case STATE_WELC:
                req->send(200, "text/html", "welc");
                break;

                default:
                break;
            }
        }       
    });

    // 404
    server->onNotFound([](AsyncWebServerRequest* req){
        Serial.printf("[SRVR]\tIn handle_404 :(\n");
    });

    server->begin();
}
