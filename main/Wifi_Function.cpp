//auteur : Hicham
//fct gestion wifi

#include <M5StickCPlus.h>
#include <WiFi.h>
#include <FS.h>
#include "SPIFFS.h"
#include <Arduino.h>

#include "./headers/Eeprom_Controller.h"
#include "./headers/Wifi_Function.h"
#include "./headers/settings.h"

bool connectToStoredWiFi()
{
    int numPairs = getNumPairs();
    DEBUG_PRINTLN("Nombre de réseaux enregistre : ");
    DEBUG_PRINTLN(numPairs);

    int numNetworks = WiFi.scanNetworks();
    DEBUG_PRINTLN("Nombre de réseaux disponibles : ");
    DEBUG_PRINTLN(numNetworks);

    //listFiles();

    // Parcourir les paires de SSID et de mot de passe
    for (int index = 0; index < numPairs; ++index)
    {
        String storedSSID, storedPassword;
        if (retrieveWiFiCredentials(storedSSID, storedPassword, index))
        {
            DEBUG_PRINTLN("Tentative de connexion à ");
            DEBUG_PRINTLN(storedSSID);

            if (tryConnectToStoredSSID(storedSSID, storedPassword, numNetworks))
            {
                return true;
            }
        }
    }

    DEBUG_PRINTLN("Aucun réseau disponible correspondant. Retour à l'état IDLE.");
    return false;
}

bool tryConnectToStoredSSID(const String &storedSSID, const String &storedPassword, int numNetworks)
{
    // Parcourir les réseaux disponibles et essayer de se connecter
    for (int i = 0; i < numNetworks; ++i) 
    {
        String scannedSSID = WiFi.SSID(i);
        // DEBUG_PRINTLN("Réseau disponible : ");
        // DEBUG_PRINTLN(scannedSSID);

        if (scannedSSID == storedSSID)
        {
            DEBUG_PRINTLN("Correspondance trouvée! Tentative de connexion...");

            if (tryConnectToWiFi(scannedSSID.c_str(), storedPassword.c_str()))
            {
                DEBUG_PRINTLN("Connexion réussie!");
                return true; // La connexion a réussi
            }
        }
    }

    DEBUG_PRINTLN("Aucun réseau disponible correspondant à cet enregistrement.");
    return false; // Aucune connexion réussie
}

bool tryConnectToWiFi(const char *ssid, const char *password)
{
    char count = 0;
    WiFi.mode(WIFI_STA);
    WiFi.begin("Oui-fi", "Hicham123");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        DEBUG_PRINTLN(".");
        count++;
        // if (count >= CONNECTION_MAX_DELAY_WAIT)
        // {
        //     DEBUG_PRINTLN("Échec de la connexion. Essayer avec le prochain réseau...");
        //     // Ajoutez un délai si nécessaire avant de tenter la connexion suivante
        //     return false; // La connexion a échoué
        // }
    }
    DEBUG_PRINTLN("Credentials known valid ");
    return true;
}
