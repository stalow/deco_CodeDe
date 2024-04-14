// Auteur : hicham
// fonction conversion en seconde
// conversion en string depuis le tableau
// fonction pour stocker le temps compté dans un tableau

#include <cstdio>
#include "./headers/Count.h"
#include "./headers/settings.h"



unsigned long Tab_Temp[6]; // tableau compteur
unsigned long Temp_ecoule = 0;

unsigned long number_of_second(int heures, int minutes, int secondes)
{
  return (heures * 3600) + (minutes * 60) + secondes;
}

void Count_time(unsigned char face, unsigned long time)
{
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

char *longToString(unsigned long num)
{
  char *str = (char *)malloc(20); // Allouer de la mémoire pour la chaîne (assez grande pour contenir un long)
  if (str == NULL)
  {
    DEBUG_PRINTLN("Erreur d'allocation mémoire");
    exit(EXIT_FAILURE);
  }
  sprintf(str, "%ld", num); // Convertir le long en chaîne de caractères
  free(str);
  return str;
}