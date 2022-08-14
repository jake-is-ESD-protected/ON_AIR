#include "mywebserver.h"
#include "mailbox.h"
#include "states.h"
#include "mainloop.h"
#include "index.h"

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

    server.on("/", handle_onConnect);
    server.on("/idle", handle_idle);
    server.on("/woex", handle_woex);
    server.on("/meet", handle_meet);
    server.on("/reco", handle_reco);
    server.on("/wait", handle_wait);
    server.on("/welc", handle_welc);
    server.onNotFound(handle_NotFound);
    server.on("/getBell", handle_getBell);

    server.begin();

    xTaskCreate(wsClientHandler,
        "client handler",
        4096,
        NULL,
        1,
        &tClientHandler);  
}


void webserver::printIP()
{
    Serial.printf("[SRVR]\tIP address: ");
    Serial.println(WiFi.localIP());
}


void handle_onConnect()
{
    Serial.println("[SRVR]\tIn handle_onConnect\n");
    ws.server.send(200, "text/html", ws_index);
}


void handle_idle()
{
    Serial.println("[SRVR]\tIn handle_idle\n");
    ws.handle(STATE_IDLE);
}


void handle_woex()
{
    Serial.println("[SRVR]\tIn handle_woex\n");
    ws.handle(STATE_WOEX);
}


void handle_meet()
{
    Serial.println("[SRVR]\tIn handle_meet\n");
    ws.handle(STATE_MEET);
}


void handle_reco()
{
    Serial.println("[SRVR]\tIn handle_reco\n");
    ws.handle(STATE_RECO);
}


void handle_wait()
{
    Serial.println("[SRVR]\tIn handle_wait\n");
    ws.handle(STATE_WAIT);
}


void handle_welc()
{
    Serial.println("[SRVR]\tIn handle_welc\n");
    ws.handle(STATE_WELC);
}


void handle_NotFound()
{
    
}


void handle_getBell()
{
    Serial.println("[SRVR]\tIn handle_getBell\n");
    // you need to set a readable var to true if a bell occured
    // in hardware
    ws.handle(STATE_BELL);
}


void webserver::handle(uint8_t state)
{
    if(state == STATE_BELL)
    {   // talk back to client
        ws.server.send(200, "text/html", "bell");
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
}


void webserver::run()
{
    mbox.wait();
    while(mbox.data_avail())
    {
        handle(mbox.pop(NON_BLOCKING).content);
    }
}