#ifndef Gestion__RTC__H
#define Gestion__RTC__H


struct RTCData {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};

void getRTCData(RTCData* rtcData);
void set_local_time();

#endif