
#include <Arduino.h>
#include "rtc_time.h"
#include "lights.h"
#include "display.h"


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Call this in setup()
void initDisplay() {
	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		Serial.println(F("SSD1306 allocation failed"));
		for (;;);
	}
	display.clearDisplay();
	display.display();
}

// Helper to format time as HH:MM:SS
String getTimeString() {
	char buf[9];
	snprintf(buf, sizeof(buf), "%02d:%02d:%02d", currentTime.hour, currentTime.minute, currentTime.second);
	return String(buf);
}



// Call this from loop() to update display
void updateSystemDisplay(SystemStatus status) {
	display.clearDisplay();

	// Draw time in yellow area (top 16px)
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.setCursor(0, 0);
	display.print(getTimeString());

	// Draw status below
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0, 20);
	switch (status) {
		case STATUS_IDLE:
			display.println("Idle");
			break;
		case STATUS_REMOVAL_RUNNING:
			display.println("Removal Pump Active");
			break;
		case STATUS_REMOVAL_ABORTED:
			display.println("Removal Aborted: Waste Full");
			break;
		case STATUS_FILL_RUNNING:
			display.println("Fill Pump Active");
			break;
		case STATUS_FILL_ABORTED:
			display.println("Fill Aborted: Clean Empty");
			break;
		case STATUS_WASTE_FULL:
			display.println("Error: Waste Tank Full");
			break;
		case STATUS_CLEAN_EMPTY:
			display.println("Error: Clean Tank Empty");
			break;
		case STATUS_WAITING_NEXT_CYCLE:
			display.println("Waiting for Next Cycle");
			break;
		case STATUS_FEEDER1_RUNNING:
			display.println("Feeder1 Active");
			break;
		case STATUS_FEEDER2_RUNNING:
			display.println("Feeder2 Active");
			break;
		default:
			display.println("Unknown Status");
			break;
	}

	display.display();
}

