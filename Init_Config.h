#ifndef CONFIG_H
#define CONFIG_H





#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <FS.h>
#include "SPIFFS.h"
#include <WiFi.h>





extern AsyncWebServer server;
extern String ssid;
extern String password;
extern String SerieNumber;

bool checkInitFile();
void deleteInitFile();
bool checkCredentials(const String &ssid, const String &password);
bool isSafeString(const String &str);
String readHtmlFile(const char *path);
void setupWiFiAP();
void stopServerAndDisconnectWiFi();
#endif
