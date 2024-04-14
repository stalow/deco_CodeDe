
#ifndef WIFI_FUNCTIONS
#define WIFI_FUNCTIONS

#include <Arduino.h>

bool connectToStoredWiFi();

bool tryConnectToStoredSSID(const String &storedSSID, const String &storedPassword, int numNetworks);

bool tryConnectToWiFi(const char *ssid, const char *password);

#endif