#include <Arduino.h>
#include "M5StickCPlus.h"


#include "./headers/Init_Config.h"
#include "./headers/State_Flags.h"
#include "./headers/Wifi_Function.h"

#include "./headers/Send_data.h"
#include "./headers/Save_Data.h"

#include "./headers/Count.h"
#include "./headers/Accelero.h"

#include "./headers/settings.h"

enum State
{
    START,
    FIRST_START,
    WIFIPAGE,
    IDLE,
    SLEEP,
    INIT,
    COUNTING,
    CONNWIFI,
    ACK_COMPT,
    SAVE_DATA
};

State currentState = START;

RTC_TimeTypeDef RTC_TimeStruct; // RTC struct

const char *serverUrl = "http://example.com/api/endpoint";

void setup()
{
    M5.begin();
    Serial.begin(115200);
    if (!SPIFFS.begin(true))
    {
        DEBUG_PRINTLN("An Error has occurred while mounting SPIFFS");
        return;
    }
}

void loop()
{
    // LECTURE ENTREE

    M5.update();
    // lecture bouton poussoir
    if (M5.BtnA.wasReleased())
    {
        DEBUG_PRINTLN("A Pressed");
        FLAG_BTN_A_PRESSED = true;
    }

    // Face_detecte = detect_face();

    // switch ETAT
    switch (currentState)
    {
    case START:
        DEBUG_PRINTLN("En mode start");
        if (checkInitFile())
        {
            currentState = FIRST_START;
        }
        else
        {
            currentState = CONNWIFI;
        }

        break;
    case FIRST_START:
        DEBUG_PRINTLN("En mode first start");
        if (FLAG_END_FIRST_START_CONFIG == true)
        {
            currentState = CONNWIFI;
            FLAG_END_FIRST_START_CONFIG = false ; // reset
        }

        break;
    case IDLE:
        DEBUG_PRINTLN("En mode IDLE");
        if (FLAG_BTN_A_PRESSED)
        {
            currentState = COUNTING;
            FLAG_BTN_A_PRESSED = false; //reset
        }
        // Ajoutez le code correspondant pour le mode IDLE ici
        break;
    case WIFIPAGE:
        // DEBUG_PRINTLN("En mode WIFIPAGE");
        //  Ajoutez le code correspondant pour le mode WIFIPAGE ici
        break;
    case SLEEP:
        DEBUG_PRINTLN("En mode SLEEP");
        // Ajoutez le code correspondant pour le mode SLEEP ici
        break;
    case INIT:
        DEBUG_PRINTLN("En mode INIT");
        // Ajoutez le code correspondant pour le mode INIT ici
        break;
    case CONNWIFI:
        DEBUG_PRINTLN("En mode CONNWIFI");
        if (FLAG_WIFI_CONNECTED)
        {
            currentState = ACK_COMPT; // à modif pour ack
        }
        else
        {
            currentState = IDLE;
        }

        // Ajoutez le code correspondant pour le mode CONNWIFI ici
        break;
    case COUNTING:
        DEBUG_PRINTLN("En mode COUNTING");
        if (FLAG_BTN_A_PRESSED)
        {
            DEBUG_PRINTLN("appui bouton go ack ");
            currentState = CONNWIFI;
            FLAG_BTN_A_PRESSED = false; //reset
        }

        if (FLAG_BTN_OFF_PRESSED)
        {
            currentState = SAVE_DATA;
            FLAG_BTN_OFF_PRESSED = false; //reset        
        }

        // Ajoutez le code correspondant pour le mode INIT ici
        break;
    case ACK_COMPT:
        if (FLAG_ACK_DATA_FINISH)
        {
            currentState = IDLE;
        }
        // Ajoutez le code correspondant pour le mode ACK_COMPT ici
        break;
    case SAVE_DATA:
        DEBUG_PRINTLN("En mode SAVE_DATA");
        if (FLAG_DATA_SAVED)
        {
            currentState = SLEEP;
            FLAG_DATA_SAVED = false;
        }
        // Ajoutez le code correspondant pour le mode SAVE_DATA ici
        break;
    default:
        DEBUG_PRINTLN("État inconnu");
    }

    // switch SORTIE
    switch (currentState)
    {
    case START:
        DEBUG_PRINTLN("En mode Start");
        // if ()
        // {
        //     FLAG_FIRST_START = true;
        // }
        // else
        // {
        //     FLAG_FIRST_START = false;
        // }
        break;
    case FIRST_START:
        DEBUG_PRINTLN("En mode first start action");
        if (FLAG_PAGE_WIFI_STARTED == false)
        {
            setupWiFiAP();
        }
        break;
    case IDLE:
        DEBUG_PRINTLN("En mode IDLE");
        FLAG_BTN_A_PRESSED = false; // reset suite à l'appui du bouton
        // Ajoutez le code correspondant pour le mode IDLE ici
        break;
    case WIFIPAGE:
        // DEBUG_PRINTLN("En mode WIFIPAGE");

        break;
    case SLEEP:
        DEBUG_PRINTLN("En mode SLEEP");
        // Ajoutez le code correspondant pour le mode SLEEP ici
        break;
    case INIT:
        DEBUG_PRINTLN("En mode INIT");
        // Ajoutez le code correspondant pour le mode INIT ici
        break;
    case CONNWIFI:
        DEBUG_PRINTLN("En mode CONNWIFI");
        if (connectToStoredWiFi())
        {
            FLAG_WIFI_CONNECTED = true;
        }
        else
        {
            currentState = WIFIPAGE;
        }
        break;
    case COUNTING:
        DEBUG_PRINTLN("En mode COUNTING");
        DEBUG_PRINTLN(Face_detecte);
        DEBUG_PRINTLN(ancienne_face);
        if (Face_detecte == ancienne_face) // test si la face change
        {
            DEBUG_PRINTLN("no change");
        }
        else
        {
            // comptabilisation du temp passe
            M5.Rtc.GetTime(&RTC_TimeStruct);
            Temp_ecoule = number_of_second(RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);
            Count_time(ancienne_face, Temp_ecoule);
            Temp_ecoule = 0; // reset

            // reset TIME =0
            RTC_TimeTypeDef TimeStruct;
            TimeStruct.Hours = 0; // Set the time.
            TimeStruct.Minutes = 0;
            TimeStruct.Seconds = 0;
            M5.Rtc.SetTime(&TimeStruct); // writes the set time to the real time clock.

            ancienne_face = Face_detecte;
        }
        // Ajoutez le code correspondant pour le mode INIT ici
        break;
    case ACK_COMPT:
        DEBUG_PRINTLN("En mode ACK_COMPT");
        sendPostRequestAndReset(Tab_Temp, serverUrl);
        FLAG_ACK_DATA_FINISH = true;
        // Ajoutez le code correspondant pour le mode ACK_COMPT ici
        break;
    case SAVE_DATA:
        DEBUG_PRINTLN("En mode SAVE_DATA");
        saveToSPIFFS(Tab_Temp, "./Time_data.txt");
        FLAG_DATA_SAVED = true;
        break;
    default:
        DEBUG_PRINTLN("État inconnu");
    }

    delay(1000); // Ajoutez un délai pour éviter une exécution trop rapide de la boucle
}
