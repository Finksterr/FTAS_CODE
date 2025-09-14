#include <Arduino.h>
#include "pins.h"
#include "pumps.h"
#include "rtc_time.h"

// Initialize pump and float switch pins
void initPumpPins() {
	pinMode(pumpInPin, OUTPUT);
	pinMode(pumpOutPin, OUTPUT);
	pinMode(tankLevelPin, INPUT);
	pinMode(cleanLevelPin, INPUT);
	pinMode(wasteLevelPin, INPUT);
}

// Check if fill pump can run (clean water tank not empty)
bool canRunFillPump() {
	return digitalRead(cleanLevelPin) == HIGH; // HIGH means water present
}

// Run fill pump until fish tank is full (float switch triggered)
void runFillPump() {
	if (!canRunFillPump()) {
		Serial.println("Clean water tank is empty. Fill pump will not run.");
		return;
	}
	Serial.println("Running fill pump...");
	digitalWrite(pumpInPin, HIGH); // Turn on fill pump
	// Wait until fish tank float switch signals full
	while (digitalRead(tankLevelPin) == LOW) {
		if (!canRunFillPump()) {
			// Stop if clean water tank becomes empty during fill
			Serial.println("Clean water tank became empty. Stopping fill pump.");
			break;
		}
		delay(100); // Check every 100ms
	}
	digitalWrite(pumpInPin, LOW); // Turn off fill pump
	Serial.println("Fill pump stopped.");
}

// Check if removal pump can run (waste tank not full)
bool canRunRemovalPump() {
	return digitalRead(wasteLevelPin) == LOW; // LOW means not full
}

// Run removal pump for specified duration if allowed, abort if waste tank gets full
void runRemovalPump() {
	if (!canRunRemovalPump()) {
		Serial.println("Waste tank is full. Removal pump will not run.");
		return;
	}
	Serial.println("Running removal pump...");
	digitalWrite(pumpOutPin, HIGH); // Turn on removal pump
	unsigned long startTime = millis();
	while (millis() - startTime < REMOVAL_PUMP_DURATION_SEC * 1000) {
		if (!canRunRemovalPump()) {
			Serial.println("Waste tank became full during removal. Stopping removal pump.");
			break;
		}
		delay(100); // Check every 100ms
	}
	digitalWrite(pumpOutPin, LOW);  // Turn off removal pump
	Serial.println("Removal pump stopped.");
}

// Weekly water change scheduling logic
void handleWeeklyWaterChange() {
	static bool waterChangeDoneToday = false;
	// Check if current time matches scheduled time
	if (currentTime.day % 7 == WATER_CHANGE_DAY &&
		currentTime.hour == WATER_CHANGE_HOUR &&
		currentTime.minute == WATER_CHANGE_MINUTE) {
		if (!waterChangeDoneToday) {
			Serial.println("Starting weekly water change...");
			// Ensure removal pump runs completely before fill pump starts
			runRemovalPump();
			runFillPump();
			waterChangeDoneToday = true;
		}
	} else {
		waterChangeDoneToday = false;
	}
}

