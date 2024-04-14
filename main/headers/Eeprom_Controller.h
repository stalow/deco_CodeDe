

#ifndef EEPROM__CONTROLLER
#define EEPROM__CONTROLLER

void saveWiFiCredentials(const String &ssid, const String &password, int index);

bool retrieveWiFiCredentials(String &ssid, String &password, int index);

void incrementNumPairs();
int getNumPairs();
void listFiles();


#endif