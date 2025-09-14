#include "feeder.h"
#include "rtc_time.h"
#include "display.h"

// Use ESP32 LEDC for servo PWM
// channel and frequency settings
#define SERVO_FREQ 50
#define SERVO_CHANNEL1 2
#define SERVO_CHANNEL2 3
#define SERVO_RESOLUTION 16

// Helper to set servo angle using microseconds (assuming 1ms-2ms mapping)
static void writeServoAngle(int channel, float angle) {
    // Map angle 0-180 to pulse width 1000-2000us
    float us = 1000 + (angle / 180.0) * 1000.0;
    // Convert microseconds to duty for LEDC
    uint32_t duty = (uint32_t)((us / 20000.0) * ((1 << SERVO_RESOLUTION) - 1));
    ledcWrite(channel, duty);
}

void initFeeder() {
    // configure LEDC channels for servos
    ledcSetup(SERVO_CHANNEL1, SERVO_FREQ, SERVO_RESOLUTION);
    ledcAttachPin(FEEDER1_PIN, SERVO_CHANNEL1);
    ledcSetup(SERVO_CHANNEL2, SERVO_FREQ, SERVO_RESOLUTION);
    ledcAttachPin(FEEDER2_PIN, SERVO_CHANNEL2);

    // initialize servos to 0 degrees
    writeServoAngle(SERVO_CHANNEL1, 0);
    writeServoAngle(SERVO_CHANNEL2, 0);
}

// actuate feeder1 by rotating to target degrees then returning to 0
void actuateFeeder1() {
    updateSystemDisplay(STATUS_FEEDER1_RUNNING);
    // Smooth move to target and back
    float target = FEEDER1_DEGREES;
    if (target > 180) target = 180;
    const int steps = 20;
    float current = 0;
    // step up
    for (int i = 1; i <= steps; ++i) {
        current = (target * i) / steps;
        writeServoAngle(SERVO_CHANNEL1, current);
        delay(40);
    }
    // short hold at target
    delay(200);
    // step down
    for (int i = steps; i >= 0; --i) {
        current = (target * i) / steps;
        writeServoAngle(SERVO_CHANNEL1, current);
        delay(40);
    }
    updateSystemDisplay(STATUS_IDLE);
}

// actuate feeder2 by moving smoothly to target degrees and returning
void actuateFeeder2() {
    updateSystemDisplay(STATUS_FEEDER2_RUNNING);
    float target = FEEDER2_DEGREES; // use full 180 degrees for feeder2 movement
    const int steps = 20;
    float current = 0;
    // step up
    for (int i = 1; i <= steps; ++i) {
        current = (target * i) / steps;
        writeServoAngle(SERVO_CHANNEL2, current);
        delay(40);
    }
    // short hold at target
    delay(200);
    // step down
    for (int i = steps; i >= 0; --i) {
        current = (target * i) / steps;
        writeServoAngle(SERVO_CHANNEL2, current);
        delay(40);
    }
    updateSystemDisplay(STATUS_IDLE);
}

// Scheduling: feeder1 every FEEDER1_CYCLE_DAYS at FEEDER_ACT_HOUR:FEEDER_ACT_MINUTE
void handleFeederSchedule() {
    static int lastFeeder1Day = -1;
    static int lastFeeder2Day = -1;

    // feeder2: every day at noon
    if (currentTime.hour == FEEDER_ACT_HOUR && currentTime.minute == FEEDER_ACT_MINUTE) {
        if (lastFeeder2Day != currentTime.day) {
            actuateFeeder2();
            lastFeeder2Day = currentTime.day;
        }
    } else {
        // reset minute-based lock when time moves away
        if (currentTime.minute != FEEDER_ACT_MINUTE) lastFeeder2Day = lastFeeder2Day;
    }

    // feeder1: every FEEDER1_CYCLE_DAYS
    if (currentTime.hour == FEEDER_ACT_HOUR && currentTime.minute == FEEDER_ACT_MINUTE) {
        // Decide if today is a feeder1 day based on epoch day modulo cycle
        // We'll use the day-of-year modulo FEEDER1_CYCLE_DAYS as a proxy
        int doy = currentTime.day; // assumption: day increments daily; if you want day-of-year, adjust RTC code
        if ((doy % FEEDER1_CYCLE_DAYS) == 0) {
            if (lastFeeder1Day != currentTime.day) {
                actuateFeeder1();
                lastFeeder1Day = currentTime.day;
            }
        }
    }
}
