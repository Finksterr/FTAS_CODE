#pragma once

#include <Arduino.h>
#include "pins.h"

// ---------Feeder configuration--------------

// Bank1: actuate every N days (default 3)
#define FEEDER1_CYCLE_DAYS 3

// Bank1: degrees per actuation (max 180 degrees)
#define FEEDER1_DEGREES 180

// Bank2: degrees per actuation (max 180 degrees)
#define FEEDER2_DEGREES 180

// Time to actuate (24h)
#define FEEDER_ACT_HOUR 12
#define FEEDER_ACT_MINUTE 0

// -------------------------------------------

// Function prototypes
void initFeeder();
void handleFeederSchedule();
void actuateFeeder1();
void actuateFeeder2();
