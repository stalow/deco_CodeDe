// Auteur : hicham
// fonction conversion en seconde
// conversion en string depuis le tableau
// fonction pour stocker le temps compté dans un tableau

#include <cstdio>
#include "./headers/Count.h"
#include "./headers/settings.h"
#include "./headers/gestion_rtc.h"
#include "M5StickCPlus.h"
#include <FS.h>
#include "SPIFFS.h"


unsigned long Tab_Temp[6]; // tableau compteur
unsigned long Temp_ecoule = 0;

unsigned long number_of_second(struct RTCData rtcData)
{
    return rtcData.hour * 3600 + rtcData.minute * 60 + rtcData.second;
}

// Fonction pour calculer la différence de temps entre le temps actuel de la RTC et le temps enregistré
unsigned long time_difference(struct RTCData rtcData)
{
    struct RTCData currentRTCData;
    getRTCData(&currentRTCData);
    unsigned long currentSeconds = number_of_second(currentRTCData);
    unsigned long savedSeconds = number_of_second(rtcData);

    if (currentSeconds >= savedSeconds) {
        return currentSeconds - savedSeconds;
    } else {
        // Gestion du cas où le temps enregistré est postérieur au temps actuel de la RTC
        return 0;
    }
}

void Count_time(unsigned char face, unsigned long time, struct RTCData rtcMemory)
{
  // Ouvrir ou créer le fichier tempPasse.txt dans SPIFFS
  File file = SPIFFS.open("/tempPasse.txt", "a+");
  if (!file) {
      Serial.println("Erreur lors de l'ouverture du fichier tempPasse.txt dans SPIFFS");
      return;
  }

  // Récupérer l'heure actuelle de la RTC
  RTC_TimeTypeDef timeStruct;
  RTC_DateTypeDef DateStruct;
  M5.Rtc.GetTime(&timeStruct);
  M5.Rtc.GetDate(&DateStruct);
  char currentTime[25];
  sprintf(currentTime, "%02d/%02d/%04d %02d:%02d:%02d", DateStruct.Date, DateStruct.Month, DateStruct.Year,timeStruct.Hours, timeStruct.Minutes, timeStruct.Seconds);

  // Récupérer l'heure enregistrée dans la RTC mémoire
  char savedTime[25];
  sprintf(savedTime, "%02d/%02d/%04d %02d:%02d:%02d", rtcMemory.day, rtcMemory.month, rtcMemory.year,rtcMemory.hour, rtcMemory.minute, rtcMemory.second);

  // Écrire les données dans le fichier
  //Serial.println("Face %d - Date actuelle: %s, Date enregistrée: %s, Temps écoulé: %lu secondes\n", face, currentTime, savedTime, time);
  DEBUG_PRINTLN("Face");
  DEBUG_PRINTLN(face);
  DEBUG_PRINTLN("currentTime");
  DEBUG_PRINTLN(currentTime);
  DEBUG_PRINTLN("Date enregistrée");
  DEBUG_PRINTLN(savedTime);
  DEBUG_PRINTLN("Temps écoulé");
  DEBUG_PRINTLN(time);
  file.printf("Face %d - Date actuelle: %s, Date enregistrée: %s, Temps écoulé: %lu secondes\n", face, currentTime, savedTime, time);

  // Fermer le fichier
  file.close();

  switch (face)
  {
  case 1:
    Tab_Temp[1] += time;
    break;
  case 2:
    Tab_Temp[2] += time;
    break;
  case 3:
    Tab_Temp[3] += time;
    break;
  case 4:
    Tab_Temp[4] += time;
    break;
  case 5:
    Tab_Temp[5] += time;
    break;
  case 0:
    break;
  default:
    break;
  }
}

// char *longToString(unsigned long num)
// {
//   char *str = (char *)malloc(20); // Allouer de la mémoire pour la chaîne (assez grande pour contenir un long)
//   if (str == NULL)
//   {
//     DEBUG_PRINTLN("Erreur d'allocation mémoire");
//     exit(EXIT_FAILURE);
//   }
//   sprintf(str, "%ld", num); // Convertir le long en chaîne de caractères
//   free(str);
//   return str;
// }