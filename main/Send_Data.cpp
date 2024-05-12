// auteur: Hicham
//fct send data to api

#include <HTTPClient.h>

#include "./headers/Send_data.h"
#include "./headers/settings.h"
#include <FS.h>
#include "SPIFFS.h"


void sendPostRequestAndReset(unsigned long Temp_Tab[6], const char *url, const char *serialNumber) {
  HTTPClient http;

  // Créer une chaîne JSON avec les valeurs du tableau et le numéro de série
  String jsonPayload = "{\"serialNumber\":\"" + String(serialNumber) + "\",";
  for (int i = 0; i < 6; ++i) {
    jsonPayload += "\"value" + String(i) + "\":" + String(Temp_Tab[i]);
    if (i < 5) {
      jsonPayload += ",";
    }
  }
  
  // Lire le contenu du fichier log.txt dans le SPIFFS
  File file = SPIFFS.open("/tempPasse.txt", "r");
  if (file) {
    jsonPayload += ",\"log\":\"";
    while (file.available()) {
      jsonPayload += (char)file.read();
    }
    jsonPayload += "\"";
    file.close();
  }
  jsonPayload += "}";

  // Commencer la requête HTTP POST
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  // Envoyer les données au serveur
  int httpCode = http.POST(jsonPayload);

  // Vérifier la réponse du serveur
  if (httpCode > 0) {
    String response = http.getString();
    DEBUG_PRINTLN("Réponse du serveur : " + response);
    // Effacer le contenu du fichier log.txt
    SPIFFS.remove("/tempPasse.txt");
    memset(Temp_Tab, 0, sizeof(Temp_Tab));
    DEBUG_PRINTLN("Tableau réinitialisé après la requête HTTP POST");
  } else {
    DEBUG_PRINTLN("Erreur lors de la requête HTTP POST");
  }

  // Fin de la requête HTTP
  http.end();

  // Réinitialiser le tableau
}