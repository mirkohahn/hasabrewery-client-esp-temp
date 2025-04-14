#include "battery_monitor.h"
#include "config.h"

#if ENABLE_BATTERY_MONITOR

#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

void setupBatteryMonitor() {
    Wire.begin(PIN_BATTERY_SDA, PIN_BATTERY_SCL);
    ina219.begin();
}

// Estimate battery percentage based on voltage (18650 cell)
float estimateBatteryPercentage(float voltage) {
    if (voltage >= 4.5) return 110.0;
    if (voltage >= 4.2) return 100.0;
    if (voltage <= 3.5) return 1.0;
    if (voltage < 1.0) return 0.0;

    // Linear scale between 3.5V (empty) and 4.2V (full)
    return ((voltage - 3.5) / (4.2 - 3.5)) * 99.0 + 1.0;
}


float get_battery_status() {
    float busVoltage = ina219.getBusVoltage_V();
    return estimateBatteryPercentage(busVoltage);
}

#else

float get_battery_status() {
    return -1.0;  // null equivalent for float
}

#endif
