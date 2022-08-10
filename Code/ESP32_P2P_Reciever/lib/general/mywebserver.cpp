#include "mywebserver.h"
#include "mailbox.h"
#include "states.h"
#include "mainloop.h"

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
    server.on("/getState", handle_getState);

    server.begin();
}


void webserver::printIP()
{
    Serial.printf("[SRVR]\tIP address: ");
    Serial.println(WiFi.localIP());
}


void handle_onConnect()
{
    Serial.println("[SRVR]\tIn handle_onConnect\n");
    ws.handle(STATE_IDLE);
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


void handle_getState()
{
    
}


void webserver::handle(uint8_t state)
{
    cmd_t c = {
        .origin = ORG_WS,
        .content = state
    };
    mbox.push(c, false);
    mbox.notify(tLoop, false);
    server.send(200, "text/html", SendHTML(state));
}


void webserver::run()
{
    server.handleClient();
}


String webserver::SendHTML(uint8_t state){


    String ptr = "<!DOCTYPE html> <html>\n";
    ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr +="<title>[ONAIR] Web Server</title>\n";
    ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
    ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
    ptr +=".button-on {background-color: #3498db;}\n";
    ptr +=".button-on:active {background-color: #2980b9;}\n";
    ptr +=".button-off {background-color: #34495e;}\n";
    ptr +=".button-off:active {background-color: #2c3e50;}\n";
    ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
    ptr +="</style>\n";
    ptr +="</head>\n";
    ptr +="<body>\n";
    ptr +="<h1>[ONAIR] Web Server</h1>\n";

    String states_txt[] = {
        "idle",
        "woex",
        "meet",
        "reco",
        "wait",
        "welc"
    };

    for(uint8_t i = 0; i < N_STATES; i++)
    {
        String onoff = "off";
        if(i == state)
        {
            onoff = "on";
        }
        ptr += "<a class=\"button button-" + onoff + "\" href=\"/" + states_txt[i] + "\">" + states_txt[i] + "</a>\n";
    }

    if(state == STATE_BELL)
    {
        bell = true;
        ptr += "<h2>BELL</h2>\n";
    }

    if(state != STATE_BELL)
    {
        bell = false;   
    }

    ptr +="</body>\n";
    ptr +="</html>\n";
    return ptr;
}
