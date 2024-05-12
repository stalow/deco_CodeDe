#include <WiFi.h>
#include "time.h"
#include <Wire.h>
#include <Arduino.h>
#include "M5StickCPlus.h"
#include "./headers/settings.h"
#include "./headers/gestion_rtc.h"


//RTC_TimeTypeDef TimeStruct;

//const long utcOffsetInSeconds = 3600;  // UTC+1:00 pour la France en hiver

// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

const char* ntpServer ="fr.pool.ntp.org";  
const long gmtOffset_sec     = 0;
const int daylightOffset_sec = 7200;


void set_local_time() {

  
  if (WiFi.status() != WL_CONNECTED) {
    DEBUG_PRINTLN("No wifi, from rtc");
  }
  else {
    configTime(gmtOffset_sec, daylightOffset_sec,ntpServer);


    // Affichage des informations récupérées du serveur NTP en mode débogage
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {  
        DEBUG_PRINTLN("Failed to obtain time");
        return;
    }
      
    Serial.println(&timeinfo,"%A, %B %d %Y %H:%M:%S");  

      // Charger l'heure dans la RTC du M5StickC Plus
    RTC_TimeTypeDef TimeStruct;
    TimeStruct.Hours = timeinfo.tm_hour;
    TimeStruct.Minutes = timeinfo.tm_min;
    TimeStruct.Seconds = timeinfo.tm_sec;
    M5.Rtc.SetTime(&TimeStruct);

    RTC_DateTypeDef DateStruct;
    DateStruct.WeekDay = timeinfo.tm_wday;
    DateStruct.Month = timeinfo.tm_mon + 1; // tm_mon commence à 0 pour janvier
    DateStruct.Date = timeinfo.tm_mday;
    DateStruct.Year = timeinfo.tm_year + 1900; // tm_year donne le nombre d'années depuis 1900
    M5.Rtc.SetDate(&DateStruct);

    // Afficher l'heure de la RTC du M5StickC Plus
    M5.Rtc.GetDate(&DateStruct);
    M5.Rtc.GetTime(&TimeStruct);
    Serial.printf("Heure RTC : %02d:%02d:%02d\n", TimeStruct.Hours, TimeStruct.Minutes, TimeStruct.Seconds);
    Serial.printf("Date RTC : %02d/%02d/%04d\n", DateStruct.Date, DateStruct.Month, DateStruct.Year);


  }
}

// Fonction pour récupérer les données de la RTC
void getRTCData(RTCData* rtcData) {
    RTC_TimeTypeDef timeStruct;
    RTC_DateTypeDef dateStruct;

    // Récupérer l'heure actuelle de la RTC
    M5.Rtc.GetTime(&timeStruct);
    M5.Rtc.GetDate(&dateStruct);

    // Stocker les données dans la structure RTCData
    rtcData->year = dateStruct.Year;
    rtcData->month = dateStruct.Month;
    rtcData->day = dateStruct.Date;
    rtcData->hour = timeStruct.Hours;
    rtcData->minute = timeStruct.Minutes;
    rtcData->second = timeStruct.Seconds;
}
