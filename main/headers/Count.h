#ifndef COUNT__FUNCTIONS
#define COUNT__FUNCTIONS


extern unsigned long Tab_Temp[6]; // tableau compteur
extern unsigned long Temp_ecoule;

unsigned long number_of_second(struct RTCData rtcData);
unsigned long time_difference(struct RTCData rtcData);
// char *longToString(unsigned long num);
void Count_time(unsigned char face, unsigned long time, struct RTCData rtcMemory);

#endif
