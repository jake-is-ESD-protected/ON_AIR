#include "webserver.h"
#include "mailbox.h"
#include "states.h"
#include "mainloop.h"

webserver ws;


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
    Serial.printf("\n[SRVR]\tWiFi connected on %d s after boot.\n", (cur_time / portTICK_PERIOD_MS));
    printIP();
    server.begin();
}


void webserver::printIP()
{
    Serial.printf("[SRVR]\tIP address: ");
    Serial.println(WiFi.localIP());
}


void webserver::run()
{
    WiFiClient client = server.available();
    if(client) 
    {
        cur_time = millis();
        prev_time = cur_time;
        String currentLine = "";

        while (client.connected() && cur_time - prev_time <= WEBSERVER_TIMEOUT) 
        {
            cur_time = millis();
            if (client.available())
            {
                char c = client.read();
                Serial.write(c);
                header += c;
                
                if (c == '\n')
                {
                    if (currentLine.length() == 0) 
                    {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();
            
                        if(header.indexOf("GET /IDLE/on") >= 0)
                        {
                            Serial.println("[SRVR]\tidle on");
                            idle_state = "on";
                            woex_state = "off";
                        } 
                        else if(header.indexOf("GET /IDLE/off") >= 0) 
                        {
                            Serial.println("[SRVR]\tidle off");
                            idle_state = "off";
                        } 
                        else if(header.indexOf("GET /WOEX/on") >= 0) 
                        {
                            Serial.println("[SRVR]\twoex on");
                            woex_state = "on";
                            idle_state = "off";
                        } 
                        else if(header.indexOf("GET /WOEX/off") >= 0) 
                        {
                            Serial.println("[SRVR]\twoex off");
                            woex_state = "off";
                        }
            
                        // Display the HTML web page
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");

                        // CSS
                        client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                        client.println(".button { background-color: #555555; border: none; color: white; padding: 16px 40px;");
                        client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                        client.println(".button2 {background-color: #4CAF50;}</style></head>");
            
                        // Web Page Heading
                        client.println("<body><h1>[ONAIR] Web Server</h1>");
            
                        // IDLE-button 
                        if (idle_state=="off") 
                        {
                            client.println("<p><a href=\"/IDLE/on\"><button class=\"button\">IDLE</button></a></p>");
                        } 
                        else 
                        {
                            client.println("<p><a href=\"/IDLE/off\"><button class=\"button button2\">IDLE</button></a></p>");
                            mbox.push({.origin=ORG_SW, .content=STATE_IDLE}, false);
                        } 
               
                        // WOEX-button   
                        if (woex_state=="off") 
                        {
                            client.println("<p><a href=\"/WOEX/on\"><button class=\"button\">WOEX</button></a></p>");
                        } 
                        else 
                        {
                            client.println("<p><a href=\"/WOEX/off\"><button class=\"button button2\">WOEX</button></a></p>");
                            mbox.push({.origin=ORG_SW, .content=STATE_WOEX}, false);
                        }

                        mbox.notify(tLoop, false);

                        client.println("</body></html>");
                        client.println();

                        break;
                    } 
                    else 
                    {
                        currentLine = "";
                    }
                } 
                else if (c != '\r') 
                {
                    currentLine += c;
                }
            }
        }
        header = "";
        client.stop();
        Serial.println("[SRVR]\tClient disconnected.\n");
    }
}
