#include <Arduino.h>
#include "pins.h"
#include "pumps.h"
#include "rtc_time.h"

// Initialize pump and float switch pins
void initPumpPins() {
	pinMode(PUMP_IN_PIN, OUTPUT);
	pinMode(PUMP_OUT_PIN, OUTPUT);
	pinMode(TANK_LEVEL_PIN, INPUT);
	pinMode(CLEAN_LEVEL_PIN, INPUT);
	pinMode(WASTE_LEVEL_PIN, INPUT);
}

// Check if fill pump can run (clean water tank not empty)
bool canRunFillPump() {
	return digitalRead(CLEAN_LEVEL_PIN) == HIGH; // HIGH means water present
}

// Run fill pump until fish tank is full (float switch triggered)
void runFillPump() {
	if (!canRunFillPump()) {
		Serial.println("Clean water tank is empty. Fill pump will not run.");
		return;
	}
	Serial.println("Running fill pump...");
	digitalWrite(PUMP_IN_PIN, HIGH); // Turn on fill pump
	// Wait until fish tank float switch signals full
	while (digitalRead(TANK_LEVEL_PIN) == LOW) {
		if (!canRunFillPump()) {
			// Stop if clean water tank becomes empty during fill
			Serial.println("Clean water tank became empty. Stopping fill pump.");
			break;
		}
		delay(100); // Check every 100ms
	}
	digitalWrite(PUMP_IN_PIN, LOW); // Turn off fill pump
	Serial.println("Fill pump stopped.");
}

// Check if removal pump can run (waste tank not full)
bool canRunRemovalPump() {
	return digitalRead(WASTE_LEVEL_PIN) == LOW; // LOW means not full
}

// Run removal pump for specified duration if allowed, abort if waste tank gets full
void runRemovalPump() {
	if (!canRunRemovalPump()) {
		Serial.println("Waste tank is full. Removal pump will not run.");
		return;
	}
	Serial.println("Running removal pump...");
	digitalWrite(PUMP_OUT_PIN, HIGH); // Turn on removal pump
	unsigned long startTime = millis();
	while (millis() - startTime < REMOVAL_PUMP_DURATION_SEC * 1000) {
		if (!canRunRemovalPump()) {
			Serial.println("Waste tank became full during removal. Stopping removal pump.");
			break;
		}
		delay(100); // Check every 100ms
	}
	digitalWrite(PUMP_OUT_PIN, LOW);  // Turn off removal pump
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

