#include <HTTPClient.h>
#include "Send_data.h"
#include "settings.h"



void sendPostRequestAndReset(unsigned long Temp_Tab[6], const char *url) {
  HTTPClient http;

  // Créer une chaîne JSON avec les valeurs du tableau
  String jsonPayload = "{";
  for (int i = 0; i < 6; ++i) {
    jsonPayload += "\"value" + String(i) + "\":" + String(Temp_Tab[i]);
    if (i < 5) {
      jsonPayload += ",";
    }
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
  } else {
    DEBUG_PRINTLN("Erreur lors de la requête HTTP POST");
  }

  // Fin de la requête HTTP
  http.end();

  // Réinitialiser le tableau
  memset(Temp_Tab, 0, sizeof(Temp_Tab));
  DEBUG_PRINTLN("Tableau réinitialisé après la requête HTTP POST");
}