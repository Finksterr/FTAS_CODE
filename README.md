# FTAS_CODE

This project runs an aquarium automation controller on an ESP32 using PlatformIO and the Arduino framework. Features:

- Lighting control (lamp relay + day/night PWM LEDs)
- RTC-based scheduling (DS3231)
- Two pumps (removal & fill) with float-switch failsafes
- Two servo feeders (bank1: every N days; bank2: daily)
- OLED status display (SSD1306 / U602602 compatible)

## Pin mappings
All pins are defined in `include/pins.h` using ALL_CAPS names.

- LAMP_RELAY_PIN: Relay control for lamp (GPIO 5)
- DAY_LED_PIN: Day LED PWM output (GPIO 35)
- NIGHT_LED_PIN: Night LED PWM output (GPIO 32)
- PUMP_IN_PIN: Fill pump MOSFET control (GPIO 39)
- PUMP_OUT_PIN: Removal pump MOSFET control (GPIO 34)
- TANK_LEVEL_PIN: Fish tank float switch input (GPIO 2)
- CLEAN_LEVEL_PIN: Clean water tank float switch input (GPIO 0)
- WASTE_LEVEL_PIN: Waste tank float switch input (GPIO 4)
- FEEDER1_PIN: Feeder 1 servo pin (GPIO 27)
- FEEDER2_PIN: Feeder 2 servo pin (GPIO 26)

> Adjust pins in `include/pins.h` to match your wiring.

## Configuration
Key configuration macros are in `include/feeder.h` and `include/pumps.h`:

- `FEEDER1_CYCLE_DAYS` - how many days between feeder1 actuations (default 3)
- `FEEDER1_DEGREES` - degrees feeder1 will move on each actuation (max 180)
- `FEEDER2_DEGREES` - degrees feeder2 will move on each actuation (max 180)
- `FEEDER_ACT_HOUR` / `FEEDER_ACT_MINUTE` - time of day for feeders (24h)
- `REMOVAL_PUMP_DURATION_SEC` - how long removal pump runs (seconds)
- `WATER_CHANGE_DAY/HOUR/MINUTE` - weekly water change schedule

## Behavior summary
- Weekly water change at configured time: removal pump runs for configured seconds (aborts if waste tank becomes full). Then fill pump runs until tank float switch signals full (stops if clean tank empty).
- Feeder1: actuates every `FEEDER1_CYCLE_DAYS` at the configured time; smooth move to target degrees then return to 0.
- Feeder2: actuates daily at the configured time; smooth move to target degrees then return to 0.
- OLED display shows current time and status messages (idle, pumps running, feeders running, errors).

## Build & Upload
Requires PlatformIO. From project root:

```bash
# Install dependencies and build
platformio run

# Upload to ESP32 (adjust port if necessary)
platformio run --target upload
```

## Notes & Next Steps
- Consider converting `#define` macros to `constexpr` for type safety.
- Consider persisting feeder degrees or schedule to EEPROM if you want runtime adjustment.
- Current actuator code is blocking (uses `delay()`), which is fine for simple use but can be made non-blocking for better responsiveness.

If you want, I can implement any of these improvements or add a web UI for changing configuration at runtime.
