#pragma once

#include <RTClib.h>

// Structure to hold date and time components
struct DateTimeStruct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};

//Declare global variables so they can be used in other files
extern DateTimeStruct currentTime;

// Function prototypes
void initRTC();
void updateTime();

