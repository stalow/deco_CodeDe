// auteur: Hicham
// fct gestion de la récupération et enregistrement des logs wifi en spiffs

#include <FS.h>
#include "SPIFFS.h"
#include <Arduino.h>

#include "./headers/Eeprom_Controller.h"
#include "./headers/settings.h"

int getNumPairs()
{
    File file = SPIFFS.open("/wifi.txt", "r");
    if (!file)
    {
        DEBUG_PRINTLN("Erreur lors de l'ouverture du fichier wifi SPIFFS !");
        return -1; // Retourner -1 en cas d'erreur
    }

    int numLines = 0;
    while (file.available())
    {
        if (file.readStringUntil('\n') != "")
        {
            numLines++;
        }
    }

    file.close();
    DEBUG_PRINTLN("Nombre de ligne fichier wifi");
    DEBUG_PRINTLN(numLines);
    return numLines;
}

// ... Les autres fonctions sont également mises à jour ...

void saveWiFiCredentials(const String &ssid, const String &password, int index)
{
   if (!SPIFFS.exists("/wifi.txt")) {
        // Si le fichier n'existe pas, on le crée en mode "écriture"
        File file = SPIFFS.open("/wifi.txt", "w");
        if (!file)
        {
            DEBUG_PRINTLN("Erreur lors de la création du fichier SPIFFS !");
            return;
        }
        file.close(); // Fermer le fichier immédiatement après la création
    }

    File file = SPIFFS.open("/wifi.txt", "a"); // Ouvrir le fichier en mode "ajouter"
    if (!file)
    {
        DEBUG_PRINTLN("Erreur lors de l'ouverture du fichier SPIFFS !");
        return;
    }

    // Ajouter la paire ssid:password à une nouvelle ligne dans le fichier
    file.println(ssid + ":" + password);

    file.close();
    DEBUG_PRINTLN("Credentials saved in SPIFFS");
}

bool retrieveWiFiCredentials(String &ssid, String &password, int index)
{
    File file = SPIFFS.open("/wifi.txt", "r");
    if (!file)
    {
        DEBUG_PRINTLN("Erreur lors de l'ouverture du fichier SPIFFS !");
        return false;
    }
    //  // Affichage du contenu du fichier
    // while (file.available())
    // {
    //     String line = file.readStringUntil('\n');
    //     DEBUG_PRINTLN(line);
    // }

    // // Remettre le curseur au début du fichier
    // file.seek(0);

    for (int i = 0; i <= index; i++) {
        // Lire chaque paire ssid:password jusqu'à atteindre l'index souhaité
        if (!file.available()) {
            // Si on atteint la fin du fichier avant d'atteindre l'index souhaité, retourner false
            file.close();
            return false;
        }
        ssid = file.readStringUntil(':');
        password = file.readStringUntil('\n');
    }

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