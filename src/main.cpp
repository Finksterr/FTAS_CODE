
#include <Arduino.h>
#include "rtc_time.h"
#include "lights.h"
#include "pumps.h"
#include "display.h"


void setup() {
    Serial.begin(115200);
    Wire.begin();

    initRTC();       // initialize the RTC
    initLampRelay(); // initialize relay pin
    initLedPins();   // initialize LED pins
    initPumpPins();  // Initialize pump and float switch pins
    initDisplay();   // initialize OLED display
}

void loop() {
    updateTime();  // refresh global currentTime

      /* Debug print for RTC time
      Serial.print(currentTime.hour);
      Serial.print(":");
      Serial.print(currentTime.minute);
      Serial.print(":");
      Serial.println(currentTime.second);
      */

    updateSystemDisplay(STATUS_IDLE); // update display with current time and status
    
    updateLampState(); // control lamp relay

   /* Debug print for lamp state
      if (digitalRead(lampRelayPin) == HIGH) {
          Serial.println("Lamp is ON");
      } else {
          Serial.println("Lamp is OFF");
      }
      */

    updateLedState();  // control day/night LED

   /* Debug print for LED state
      Serial.print("Day LED Brightness: ");
      Serial.println(ledcRead(0)); // channel 0 for day
      Serial.print("Night LED Brightness: ");
      Serial.println(ledcRead(1)); // channel 1 for night
      */
    
    handleWeeklyWaterChange(); // Weekly water change scheduling

    delay(1000);  // update once per second
}