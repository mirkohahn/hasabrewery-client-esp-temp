#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "temperature_sensor.h"
#include "config.h" 
#include "led.h"
#include "main.h"

// Define the GPIO pin for the DS18B20 sensor
#define ONE_WIRE_BUS 4  

// Create OneWire and DallasTemperature objects
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// DS18B20 device addresses
DeviceAddress brewSensor;
DeviceAddress ambientSensor;
DeviceAddress detectedSensors[5];  // Store detected sensors (up to 5)

bool brewSensorAvailable = false;
bool ambientSensorAvailable = false;
int totalSensors = 0;

// I M P O R T A N T
// **Manually update this with your exact sensor address**
DeviceAddress ambientSensorAddress = {0x28, 0x82, 0xC4, 0x58, 0x00, 0x00, 0x00, 0x63};  // Match your detected address
// the brew temp sensor address will be assigned automatically and can be changed without any issues, for example when temp sensor 
// "hardwired" and/or connected to fermentation vessel, etc

// Convert Celsius to Fahrenheit if IMPERIAL_UNITS is enabled
float convertTemperature(float tempC) {
    #if IMPERIAL_UNITS
        return (tempC * 9.0 / 5.0) + 32.0;  // Convert to °F
    #else
        return tempC;  // Keep in °C
    #endif
}

void setupTemperatureSensor() {
    conbrew_logln("🔍 Scanning for DS18B20 sensors...");

    sensors.begin();
    totalSensors = sensors.getDeviceCount();

    conbrew_log("🔎 Found DS18B20 sensors: ");
    conbrew_logln(totalSensors);

    if (totalSensors < 1) {
        conbrew_logln("❌ No DS18B20 sensor detected! Check wiring.");
        led_on("red");
        led_on("green");
        return;
    }

    // Store and print all detected sensor addresses
    for (int i = 0; i < totalSensors; i++) {
        if (sensors.getAddress(detectedSensors[i], i)) {
            conbrew_log("✅ Found Sensor: ");
            for (uint8_t j = 0; j < 8; j++) {
                char hexByte[4];
                snprintf(hexByte, sizeof(hexByte), "%02X", detectedSensors[i][j]);
                conbrew_log(hexByte);
                conbrew_log(" ");
            }
            
            Serial.println();
        } else {
            conbrew_logln("⚠️ Failed to retrieve DS18B20 sensor address.");
        }
    }

    // Step 1: Assign Ambient Sensor **First**
    for (int i = 0; i < totalSensors; i++) {
        if (memcmp(detectedSensors[i], ambientSensorAddress, 7) == 0) { // Compare first 7 bytes
            conbrew_logln("🌡️ Assigned as Ambient Temperature Sensor.");
            memcpy(ambientSensor, detectedSensors[i], sizeof(DeviceAddress));
            ambientSensorAvailable = true;
        }
    }

    // Step 2: Assign Brew Sensor (First Sensor That is Not Ambient)
    for (int i = 0; i < totalSensors; i++) {
        if (!brewSensorAvailable && memcmp(detectedSensors[i], ambientSensorAddress, 7) != 0) {
            conbrew_logln("🍺 Assigned as Brew Temperature Sensor.");
            memcpy(brewSensor, detectedSensors[i], sizeof(DeviceAddress));
            brewSensorAvailable = true;
        }
    }

    if (!ambientSensorAvailable) {
        conbrew_logln("⚠️ No specific ambient sensor found. Ambient Temp will return null.");
    }

    if (!brewSensorAvailable) {
        conbrew_logln("⚠️ No specific brew sensor found. Brew Temp will return null.");
    }
}

bool isSensorConnected() {
    return brewSensorAvailable;
}

// Function to read brew temperature
float readBrewTemperature() {
    if (!brewSensorAvailable) {
        conbrew_logln("⚠️ No DS18B20 sensor detected for Brew Temp. Returning null.");
        return -127.00;
    }

    sensors.requestTemperatures();
    float temperature = sensors.getTempC(brewSensor);

    if (temperature == -127.00) {
        conbrew_logln("⚠️ DS18B20 Brew Sensor Read Error: Retrying...");
        delay(500);
        sensors.requestTemperatures();
        temperature = sensors.getTempC(brewSensor);
    }

    return convertTemperature(temperature);  // Convert based on IMPERIAL_UNITS setting
}

// Function to read ambient temperature
float readAmbientTemperature() {
    if (!ambientSensorAvailable) {
        conbrew_logln("⚠️ No DS18B20 sensor detected for Ambient Temp. Returning null.");
        return -127.00;
    }

    sensors.requestTemperatures();
    float temperature = sensors.getTempC(ambientSensor);

    if (temperature == -127.00) {
        conbrew_logln("⚠️ DS18B20 Ambient Sensor Read Error: Retrying...");
        delay(500);
        sensors.requestTemperatures();
        temperature = sensors.getTempC(ambientSensor);
    }

    return convertTemperature(temperature);  // Converted based on IMPERIAL_UNITS setting
}
