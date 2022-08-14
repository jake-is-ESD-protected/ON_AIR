#include "mywebserver.h"
#include "mailbox.h"
#include "states.h"
#include "tasks.h"
#include "index.h"
#include "core.h"
#include "AsyncElegantOTA.h"

webserver ws;
bool bell = false;


webserver::webserver()
{
    cur_time = millis();
}


void webserver::init()
{
    Serial.printf("[SRVR]\tConnecting to %s ", SSID);
    WiFi.begin(SSID, PWD);
    while (WiFi.status() != WL_CONNECTED) {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        Serial.printf(".");
    }
    Serial.printf("\n[SRVR]\tWiFi connected on %d ms after boot.\n", 
                (uint32_t)((millis() - cur_time) / portTICK_PERIOD_MS));
    printIP();

    // index
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* req){
        Serial.println("[SRVR]\tIn handle_onConnect\n");
        req->send(200, "text/html", ws_index);
    });

    // idle
    server.on("/idle", HTTP_GET, [&](AsyncWebServerRequest* req){

        Serial.println("[SRVR]\tIn handle_idle\n");
        handle(STATE_IDLE);

    });

    // woex
    server.on("/woex", HTTP_GET, [&](AsyncWebServerRequest* req){
        if(cur_state != STATE_WOEX)
        {
            Serial.println("[SRVR]\tIn handle_woex\n");
            handle(STATE_WOEX);
        }
    });

    // meet
    server.on("/meet", HTTP_GET, [&](AsyncWebServerRequest* req){
        if(cur_state != STATE_MEET)
        {
            Serial.println("[SRVR]\tIn handle_meet\n");
            handle(STATE_MEET);
        }
    });

    // reco
    server.on("/reco", HTTP_GET, [&](AsyncWebServerRequest* req){
        if(cur_state != STATE_RECO)
        {
            Serial.println("[SRVR]\tIn handle_reco\n");
            handle(STATE_RECO);
        }
    });

    // wait
    server.on("/wait", HTTP_GET, [&](AsyncWebServerRequest* req){
        if(cur_state != STATE_WAIT)
        {
            Serial.println("[SRVR]\tIn handle_wait\n");
        handle(STATE_WAIT);
        }
    });

    // welc
    server.on("/welc", HTTP_GET, [&](AsyncWebServerRequest* req){
        if(cur_state != STATE_WELC)
        {
            Serial.println("[SRVR]\tIn handle_welc\n");
            handle(STATE_WELC);
        }
    });

    // getBell
    server.on("/getBell", HTTP_GET, [&](AsyncWebServerRequest* req){
        if(core.volatile_state == STATE_BELL)
        {
            handle(STATE_BELL);
        }
    });

    // 404
    server.onNotFound([](AsyncWebServerRequest* req){
        Serial.println("[SRVR]\tIn handle_404\n");
        req->send(200, "text/plain", "404 page not found");
    });


    AsyncElegantOTA.begin(&server);
    server.begin();
}


void webserver::printIP()
{
    Serial.printf("[SRVR]\tIP address: ");
    Serial.println(WiFi.localIP());
}


void webserver::handle(uint8_t state)
{
    if(state == STATE_BELL)
    {   // talk back to client
        [&](AsyncWebServerRequest* req){
            req->send(200, "text/html", "bell");
        };
    }
    else
    {   // process client command
        cmd_t c = {
            .origin = ORG_WS,
            .content = state
        };
        mbox.push(c, false);
        mbox.notify(tLoop, false);
    }

    cur_state = state;
}


void webserver::run()
{
    mbox.wait();
    while(mbox.data_avail())
    {
        handle(mbox.pop(NON_BLOCKING).content);
    }
}