// auteur: Hicham
// fct gestion de la récupération et enregistrement des logs wifi en spiffs

#include <FS.h>
#include "SPIFFS.h"
#include <Arduino.h>

#include "./headers/Eeprom_Controller.h"
#include "./headers/settings.h"

void incrementNumPairs()
{
    File file = SPIFFS.open("/config.txt", "r+");
    if (!file)
    {
        DEBUG_PRINTLN("Erreur lors de l'ouverture du fichier SPIFFS !");
        return;
    }

    int numPairs = file.read();
    DEBUG_PRINTLN("numpaire trouve");
    DEBUG_PRINTLN(numPairs);
    numPairs++;

    file.seek(0);
    file.write(numPairs);

    file.close();
}

int getNumPairs()
{
    File file = SPIFFS.open("/config.txt", "r");
    if (!file)
    {
        DEBUG_PRINTLN("Le fichier /config.txt n'existe pas. Création du fichier avec la valeur 0.");
        file = SPIFFS.open("/config.txt", "w");
        if (!file)
        {
            DEBUG_PRINTLN("Erreur lors de la création du fichier SPIFFS !");
            return 0; // Ou une valeur par défaut appropriée
        }

        file.print(0);
        file.close();

        return 0; // La valeur par défaut après la création du fichier
    }

    int numPairs = file.read();
    DEBUG_PRINTLN("numpaire trouve");
    DEBUG_PRINTLN(numPairs);
    file.close();

    return numPairs;
}


// ... Les autres fonctions sont également mises à jour ...

void saveWiFiCredentials(const String &ssid, const String &password, int index)
{
    String fileName = "/pair_" + String(index) + ".txt";
    DEBUG_PRINTLN("nom du fichier qui sera créer ");
    DEBUG_PRINTLN(fileName);
    File file = SPIFFS.open(fileName, "w");
    if (!file)
    {
        DEBUG_PRINTLN("Erreur lors de l'ouverture du fichier SPIFFS !");
        return;
    }

    file.print(ssid);
    file.print('\0'); // Null-terminate le SSID
    file.print(password);
    file.print('\0'); // Null-terminate le mot de passe

    file.close();
    DEBUG_PRINTLN("log saved in SPIFFS in index");
    DEBUG_PRINTLN(index);
    incrementNumPairs();
}

bool retrieveWiFiCredentials(String &ssid, String &password, int index)
{
    String fileName = "/pair_" + String(index) + ".txt";
    File file = SPIFFS.open(fileName, "r");
    if (!file)
    {
        DEBUG_PRINTLN("Erreur lors de l'ouverture du fichier SPIFFS !");
        return false;
    }

    ssid = file.readStringUntil('\0');
    password = file.readStringUntil('\0');

    file.close();
    DEBUG_PRINTLN("log found");
    DEBUG_PRINTLN(ssid);
    DEBUG_PRINTLN(password);
    return true;
}


void listFiles() {
  File root = SPIFFS.open("/");
  if (!root) {
    Serial.println("Erreur lors de l'ouverture du répertoire SPIFFS !");
    return;
  }

  Serial.println("Liste des fichiers SPIFFS :");
  File file = root.openNextFile();
  while (file) {
    Serial.print("  * ");
    Serial.println(file.name());
    file = root.openNextFile();
  }
}