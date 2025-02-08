#include <Arduino.h>
#include "main.h"
#include <WiFi.h>
#include "config.h"
#include "mqtt_handler.h"
#include "temperature_sensor.h"
#include "esp_sleep.h"

// FYI: Serial.println are for debugging purposes, and can be removed if not needed
// prints shouldnt effect the battery efficiency too much / noticable 
// can be used, when connection / flashing thru VSC / PlatformIO and monitoring the serial monitor


RTC_DATA_ATTR int bootCount = 0; // Retains count across deep sleep cycles

void connectToStrongestWiFi() {
    Serial.println("🔍 Scanning for WiFi networks...");
    int numNetworks = WiFi.scanNetworks();

    if (numNetworks == 0) {
        Serial.println("❌ No WiFi networks found!");
        return;
    }

    int bestNetworkIndex = -1;
    int bestRSSI = -100; // Start with the lowest possible signal strength

    // Find the strongest network that matches a known SSID
    for (int i = 0; i < numNetworks; i++) {
        String detectedSSID = WiFi.SSID(i);
        int signalStrength = WiFi.RSSI(i);

        Serial.printf("📡 Found: %s (RSSI: %d dBm)\n", detectedSSID.c_str(), signalStrength);

        // Check if the detected network matches our predefined SSID
        if (detectedSSID == WIFI_SSID && signalStrength > bestRSSI) {
            bestRSSI = signalStrength;
            bestNetworkIndex = i;
        }
    }

    // Connect to the best available network
    if (bestNetworkIndex != -1) {
        Serial.printf("✅ Connecting to best WiFi: %s (RSSI: %d dBm)...\n", WiFi.SSID(bestNetworkIndex).c_str(), bestRSSI);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        int attempt = 0;
        while (WiFi.status() != WL_CONNECTED && attempt < 10) { // Limit attempts
            delay(500);
            Serial.print(".");
            attempt++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\n✅ WiFi connected!");
            Serial.print("📡 IP Address: ");
            Serial.println(WiFi.localIP());
        } else {
            Serial.println("\n❌ WiFi connection failed!");
        }
    } else {
        Serial.println("⚠️ No known strong network found. Using default WiFi credentials.");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }
}

void setup()
{
    Serial.begin(115200);
    delay(100);

    // Connect to the strongest available WiFi
    connectToStrongestWiFi();

    // Sync time using NTP
    setupTime();

    // Initialize MQTT
    mqttInit();

    // Initialize DS18B20 sensor
    setupTemperatureSensor();
}

unsigned long previousMillis = 0;

void loop()
{
    mqttLoop();

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= INTERVAL_LENGTH * 1000UL)
    {
        previousMillis = currentMillis;

        // Read temperatures
        float brewTemp = readBrewTemperature();
        float ambientTemp = readAmbientTemperature();

        Serial.print("📡 Brew Temperature: ");
        Serial.print(brewTemp);
        Serial.println(" °C");          // Print Brew Temp for Debugging Purposes - Hence not using IMPERIAL_UNITS

        Serial.print("📡 Ambient Temperature: ");
        Serial.print(ambientTemp);
        Serial.println(" °C");          // Print Ambient Temp for Debugging Purposes - Hence not using IMPERIAL_UNITS

        // Publish both temperatures
        publishTemperature(brewTemp, ambientTemp); 

        Serial.println("🛑 Waiting 2 seconds to ensure MQTT message is sent...");
        delay(2000); // Ensure MQTT message gets sent before sleeping

        Serial.println("💤 Setting Deep Sleep Timer - Goodnight!");
        esp_sleep_enable_timer_wakeup(INTERVAL_LENGTH * 1000000ULL);
        esp_deep_sleep_start();
    }
}
