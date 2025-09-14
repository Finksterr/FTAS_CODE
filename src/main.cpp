#include <Arduino.h>
#include "rtc_time.h"

void setup() {
    Serial.begin(115200);
    Wire.begin();

    initRTC();  // initialize the RTC
}

void loop() {
    updateTime();  // refresh global currentTime

    // Debug print
    Serial.print(currentTime.hour);
    Serial.print(":");
    Serial.print(currentTime.minute);
    Serial.print(":");
    Serial.println(currentTime.second);

    delay(1000);  // update once per second
}