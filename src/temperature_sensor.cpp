#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "temperature_sensor.h"

// Define the GPIO pin for the DS18B20 sensor
#define ONE_WIRE_BUS 4  // Change if using a different GPIO pin

// Create OneWire and DallasTemperature objects
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// DS18B20 device address
DeviceAddress brewSensor;
bool sensorAvailable = false;  // Track sensor presence

void setupTemperatureSensor() {
    Serial.println("🔍 Scanning for DS18B20 sensors...");
    
    sensors.begin();
    int sensorCount = sensors.getDeviceCount();

    Serial.print("🔎 Found DS18B20 sensors: ");
    Serial.println(sensorCount);

    if (sensorCount < 1) {
        Serial.println("❌ No DS18B20 sensor detected! Check wiring.");
        sensorAvailable = false;
        return;
    }

    if (!sensors.getAddress(brewSensor, 0)) {
        Serial.println("❌ Failed to retrieve DS18B20 sensor address.");
        sensorAvailable = false;
        return;
    }

    // Sensor detected successfully
    sensorAvailable = true;
    Serial.print("✅ DS18B20 Sensor Address: ");
    for (uint8_t i = 0; i < 8; i++) {
        Serial.print(brewSensor[i], HEX);
    }
    Serial.println();
}

bool isSensorConnected() {
    return sensorAvailable;
}

// Function to read brew temperature
float readBrewTemperature() {
    if (!sensorAvailable) {
        Serial.println("⚠️ No DS18B20 sensor detected. Returning null temperature.");
        return -127.00;  // Indicating error
    }

    sensors.requestTemperatures();
    float temperature = sensors.getTempC(brewSensor);

    // Retry in case of error (-127°C is a failure code)
    if (temperature == -127.00) {
        Serial.println("⚠️ DS18B20 Read Error: Retrying...");
        delay(500);
        sensors.requestTemperatures();
        temperature = sensors.getTempC(brewSensor);
    }

    return temperature;
}
