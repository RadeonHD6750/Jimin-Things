
#ifndef SERVER_H
#define SERVER_H

#include <ESP8266WebServer.h>

//HTML 소스들
#include "HTMLPage.h"

String getConnectedSSID();
String getConnectedPasswordD();
ESP8266WebServer* getESP8266WebServer();

void startServer();
void scanWiFiList();
void indexController();
void wifiListController();
String prepareSelectWifiListPage();

#endif
