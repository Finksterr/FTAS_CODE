#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display dimensions and reset pin
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

extern Adafruit_SSD1306 display;

// System status enum for display
enum SystemStatus {
    STATUS_IDLE,
    STATUS_REMOVAL_RUNNING,
    STATUS_REMOVAL_ABORTED,
    STATUS_FILL_RUNNING,
    STATUS_FILL_ABORTED,
    STATUS_WASTE_FULL,
    STATUS_CLEAN_EMPTY,
    STATUS_WAITING_NEXT_CYCLE
};

// Call in setup()
void initDisplay();

// Call in loop() to update display
void updateSystemDisplay(SystemStatus status);

// Helper to format time
String getTimeString();
