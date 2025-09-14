
#include <Arduino.h>
#include "lights.h"
#include "rtc_time.h"
#include "pins.h"


// Setup relay pin
void initLampRelay() {
	pinMode(lampRelayPin, OUTPUT);
	digitalWrite(lampRelayPin, LOW); // Start with lamp OFF
}

// Setup LED pins for PWM
void initLedPins() {
	pinMode(dayLedPin, OUTPUT);
	pinMode(nightLedPin, OUTPUT);
	// Attach pins to PWM channels
	ledcAttachPin(dayLedPin, 0); // channel 0 for day
	ledcAttachPin(nightLedPin, 1); // channel 1 for night
	ledcSetup(0, 5000, 8); // channel 0, 5kHz, 8-bit
	ledcSetup(1, 5000, 8); // channel 1, 5kHz, 8-bit
	ledcWrite(0, 0);
	ledcWrite(1, 0);
}

// Fade helper for smooth transitions
void fadeLed(uint8_t channel, uint8_t target, uint8_t step = 5, uint16_t delayMs = 10) {
	int value = ledcRead(channel);
	if (value < target) {
		for (int v = value; v <= target; v += step) {
			ledcWrite(channel, v);
			delay(delayMs);
		}
	} else if (value > target) {
		for (int v = value; v >= target; v -= step) {
			ledcWrite(channel, v);
			delay(delayMs);
		}
	}
	ledcWrite(channel, target);
}

// Update lamp state based on time
void updateLampState() {
	// Define day as 7:00 to 19:00
	int hour = currentTime.hour;
	if (hour >= 7 && hour < 19) {
		digitalWrite(lampRelayPin, HIGH); // Lamp ON
	} else {
		digitalWrite(lampRelayPin, LOW);  // Lamp OFF
	}
}

// Update LED state with fade
void updateLedState() {
	int hour = currentTime.hour;
	if (hour >= 7 && hour < 19) {
		// Day: fade in day LED, fade out night LED
		fadeLed(0, 255); // day full brightness
		fadeLed(1, 0);   // night off
	} else {
		// Night: fade in night LED, fade out day LED
		fadeLed(0, 0);   // day off
		fadeLed(1, 255); // night full brightness
	}
}
