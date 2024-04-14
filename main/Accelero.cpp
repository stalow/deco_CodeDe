// Auteur : hicham
// fonction gestion accelero /setup 
// fonction pour detecter la face du haut



#include "M5StickCPlus.h"


#include "./headers/Accelero.h"
#include "./headers/settings.h"

unsigned char ancienne_face = 0; // face veille pour init
unsigned char Face_detecte = 0;


float accX = 0;
float accY = 0;
float accZ = 0;



void setup_IMU(void){
    M5.Imu.Init(); // Init IMU.
}


unsigned char detect_face()
{
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  //DEBUG_PRINTLN("***************************************************");
  // Serial.printf(" X= %5.2f   Y= %5.2f   Z= %5.2f\n", accX, accY, accZ);

  if (-0.2 <= accX && accX <= 0.2 &&
      -0.2 <= accY && accY <= 0.2 &&
      -1.2 <= accZ && accZ <= -0.8)
  {
   DEBUG_PRINTLN("face1");
    return 1;
  }
  else if (-0.2 <= accX && accX <= 0.2 &&
           0.8 <= accY && accY <= 1.2 &&
           -0.2 <= accZ && accZ <= 0.2)
  {
   DEBUG_PRINTLN("face2");
    return 2;
  }
  else if ((-1.2 <= accX && accX <= -0.8) &&
           (-0.2 <= accY && accY <= 0.2) &&
           (-0.2 <= accZ && accZ <= 0.2))
  {
   DEBUG_PRINTLN("face3");
    return 3;
  }
  else if ((0.8 <= accX && accX <= 1.2) &&
           (-0.2 <= accY && accY <= 0.2) &&
           (-0.2 <= accZ && accZ <= 0.2))
  {
   DEBUG_PRINTLN("face4");
    return 4;
  }
  else if ((-0.2 <= accX && accX <= 0.2) &&
           (-1.2 <= accY && accY <= -0.8) &&
           (-0.2 <= accZ && accZ <= 0.2))
  {
   DEBUG_PRINTLN("face5");
    return 5;
  }
  else if ((-0.2 <= accX && accX <= 0.2) &&
           (-0.2 <= accY && accY <= 0.2) &&
           (0.8 <= accZ && accZ <= 1.2))
  {
   DEBUG_PRINTLN("face veille");
    return 0;
  }
  else
  {
   DEBUG_PRINTLN("Aucune face detecte");
    return 66;
  }
}