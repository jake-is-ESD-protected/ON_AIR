#ifndef _MYWEBSERVER_H_
#define _MYWEBSERVER_H_

#include <WiFi.h>
#include <WebServer.h>
#include <ESPAsyncWebServer.h>

#define WEBSERVER_TIMEOUT   2000
#define BUFLEN              30

/// @brief Prints obtained IP to Serial
/// @note Will only print useful information if device
/// is connected to WiFi (duh)
void mywebserver_printIP();

/// @brief Initializes the webserver hosting the control page
/// for clients within the same LAN
/// @note Will block until connected, because this device is
/// useless without internet connection. Check the folder
/// "website" for its html, css and js content. Uses ajax for
/// page updates.
void mywebserver_init();

#endif