#include <Arduino.h>
#include "main.h"
#include <WiFi.h>
#include "config.h"
#include "mqtt_handler.h"
#include "temperature_sensor.h"

void setup() {
    Serial.begin(115200);
    delay(100);

    // Connect to WiFi
    Serial.println();
    Serial.print("Connecting to WiFi network: ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\n✅ WiFi connected.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Sync time using NTP
    setupTime();  // Now this function is properly declared

    // Initialize MQTT
    mqttInit();

    // Initialize DS18B20 sensor
    setupTemperatureSensor();
}


unsigned long previousMillis = 0;

void loop() {
    mqttLoop();

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= INTERVAL_LENGTH * 1000UL) {
        previousMillis = currentMillis;

        // Read real temperature from the brew sensor
        float brewTemp = readBrewTemperature();

        Serial.print("Brew Temperature: ");
        Serial.print(brewTemp);
        Serial.println(" °C");

        // Publish brew temperature, setting ambient temp to null
        publishTemperature(brewTemp);
    }
}
