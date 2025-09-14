#pragma once

// Pump and float switch pin definitions
#include "pins.h"

// Adjustable timing for removal pump (seconds)
#define REMOVAL_PUMP_DURATION_SEC 30 // Change as needed

// Weekly schedule (day of week, hour, minute)
#define WATER_CHANGE_DAY 0    // 0 = Sunday
#define WATER_CHANGE_HOUR 10  // 10 AM
#define WATER_CHANGE_MINUTE 0 // 0 minutes

// Function prototypes
bool canRunRemovalPump();
bool canRunFillPump();
void runRemovalPump();
void runFillPump();
void initPumpPins();
void handleWeeklyWaterChange();

