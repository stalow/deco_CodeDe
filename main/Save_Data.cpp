// auteur: hicham
// focntion save des data de comptage

#include <cstdio>
#include <FS.h>
#include "SPIFFS.h"

#include "./headers/Save_Data.h"
#include "./headers/settings.h"

void saveToSPIFFS(unsigned long Temp_Tab[6], const char *fileName) {
  // Open the file in write mode (create if it doesn't exist)
  File file = SPIFFS.open(fileName, "w");

  if (!file) {
    DEBUG_PRINTLN("Unable to open file for writing");
    return;
  }

  // Write the values of the array to the file
  for (int i = 0; i < 6; ++i) {
    file.print(Temp_Tab[i]);
    file.print("\n");
  }

  // Close the file
  file.close();
  DEBUG_PRINTLN("Data written to file successfully");
}

void loadFromSPIFFS(unsigned long Temp_Tab[6], const char *fileName) {
  // Open the file in read mode
  File file = SPIFFS.open(fileName, "r");

  if (!file) {
    DEBUG_PRINTLN("Unable to open file for reading");
    return;
  }

  // Read the values from the file and populate the array
  for (int i = 0; i < 6; ++i) {
    if (file.available()) {
      Temp_Tab[i] = file.readStringUntil('\n').toInt();
    } else {
      DEBUG_PRINTLN("End of file reached");
      break;
    }
  }

  // Close the file
  file.close();
  DEBUG_PRINTLN("Data read from file successfully");
}
