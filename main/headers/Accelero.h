#ifndef ACCELERO__FUNCTION
#define ACCELERO__FUNCTION

extern unsigned char ancienne_face; 
extern unsigned char Face_detecte;


// variable IMU accel
extern float accX;
extern float accY;
extern float accZ;

void setup_IMU();
unsigned char detect_face();



#endif