#ifndef GLOBAL__SETTING
#define GLOBAL__SETTING

#include <Arduino.h>

#define DEBUG_MODE 1 //  1 pour activer le mode debug, 0 pour le désactiver

#if DEBUG_MODE
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINTLN(x) // Si DEBUG_MODE est désactivé, cette macro est vide
#endif

#define CONNECTION_MAX_DELAY_WAIT 10 // 3*500ms


#endif