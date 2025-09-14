#include <Arduino.h>
#include <Wire.h>
#include "rtc_time.h"

RTC_DS3231 rtc;              // create RTC object
DateTimeStruct currentTime;  // define global time variable

// Initialize RTC
void initRTC() {
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1); // hang if no RTC found
    }

    // Only set RTC once (first time you program), then comment this out
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("RTC initialized to compile time.");
}

// Update current time from RTC
void updateTime() {
    DateTime now = rtc.now();
    currentTime.year   = now.year();
    currentTime.month  = now.month();
    currentTime.day    = now.day();
    currentTime.hour   = now.hour();
    currentTime.minute = now.minute();
    currentTime.second = now.second();
}