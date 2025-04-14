#include <Arduino.h>
#include "main.h"
#include <WiFi.h>
#include "config.h"
#include "led.h"
#include "mqtt_handler.h"
#include "temperature_sensor.h"
#include "battery_monitor.h"
#include "esp_sleep.h"

// FYI: Serial.println are for debugging purposes, and can be removed if not needed
// prints shouldnt effect the battery efficiency too much / noticable
// can be used, when connection / flashing thru VSC / PlatformIO and monitoring the serial monitor

RTC_DATA_ATTR int bootCount = 0; // Retains count across deep sleep cycles

void connectToStrongestWiFi()
{
    Serial.println("🔍 Scanning for WiFi networks...");
    led_blink("green", 5, 250, 250);
    int numNetworks = WiFi.scanNetworks();
    
    if (numNetworks == 0)
    {
        Serial.println("❌ No WiFi networks found!");
        led_off("green");
        led_on("red");
        led_blink("green", 3, 500, 500);
        return;
    }

    int bestNetworkIndex = -1;
    int bestRSSI = -100;  // Start with the lowest possible signal strength
    uint8_t bestBSSID[6]; // Store the BSSID of the strongest AP
    int bestChannel = 0;

    // Find the strongest network that matches our predefined SSID
    for (int i = 0; i < numNetworks; i++)
    {
        String detectedSSID = WiFi.SSID(i);
        int signalStrength = WiFi.RSSI(i);

        Serial.printf("📡 Found: %s (RSSI: %d dBm, Channel: %d)\n",
                      detectedSSID.c_str(), signalStrength, WiFi.channel(i));

        // Check if the detected network matches our predefined SSID
        if (detectedSSID == WIFI_SSID && signalStrength > bestRSSI)
        {
            bestRSSI = signalStrength;
            bestNetworkIndex = i;
            memcpy(bestBSSID, WiFi.BSSID(i), sizeof(bestBSSID));
            bestChannel = WiFi.channel(i);
        }
    }

    // Connect to the best available AP
    if (bestNetworkIndex != -1)
    {
        Serial.printf("✅ Connecting to strongest WiFi AP: %s (RSSI: %d dBm, Channel: %d)...\n",
                      WiFi.SSID(bestNetworkIndex).c_str(), bestRSSI, bestChannel);

        WiFi.begin(WIFI_SSID, WIFI_PASSWORD, bestChannel, bestBSSID, true);

        int attempt = 0;
        while (WiFi.status() != WL_CONNECTED && attempt < 15)
        { // Increase attempts to 15 for better stability
            delay(500);
            Serial.print(".");
            attempt++;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("\n✅ WiFi connected!");
            led_off("green");
            led_blink("green", 3, 100, 100);
            led_on("green");
            Serial.print("📡 IP Address: ");
            Serial.println(WiFi.localIP());

            Serial.print("🛰️ Connected BSSID: ");
            Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                          bestBSSID[0], bestBSSID[1], bestBSSID[2],
                          bestBSSID[3], bestBSSID[4], bestBSSID[5]);

            WiFi.setAutoReconnect(true); // Ensure it reconnects if disconnected
            WiFi.setSleep(false);        // Disable WiFi sleep for stable connection
        }
        else
        {
            Serial.println("\n❌ WiFi connection failed!");
            led_blink("red", 3, 100, 100);
            led_blink("red", 3, 250, 250);
            led_on("red");
        }
    }
    else
    {
        Serial.println("⚠️ No known strong network found. Using default WiFi credentials.");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        led_blinking("red");
    }
}

void setup()
{

    // Enabling Optional Features
#if ENABLE_LED
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
#endif

    led_on("green");
    delay(1000);
    led_on("red");
    delay(1000);
    led_on("blue");
    delay(1000);
    leds_off();

#if ENABLE_BATTERY_MONITOR
    setupBatteryMonitor();
#endif

    // Setup for Critical Features

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

#if ENABLE_LED
    led_loop();
#endif

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1 * 1000UL)
    {
        previousMillis = currentMillis;

        // Read temperatures
        float brewTemp = readBrewTemperature();
        float ambientTemp = readAmbientTemperature();

        Serial.print("📡 Brew Temperature: ");
        Serial.print(brewTemp);
        Serial.println(" °C"); // Print Brew Temp for Debugging Purposes - Hence not using IMPERIAL_UNITS

        Serial.print("📡 Ambient Temperature: ");
        Serial.print(ambientTemp);
        Serial.println(" °C"); // Print Ambient Temp for Debugging Purposes - Hence not using IMPERIAL_UNITS

        // Publish both temperatures
        publishTemperature(brewTemp, ambientTemp);

        Serial.println("🛑 Waiting 2 seconds to ensure MQTT message is sent...");
        delay(2000); // Ensure MQTT message gets sent before sleepingt


        // OPTIONAL - Deep Sleep disconnects all Power Consumers and reduces to ~50µA
        // esp_sleep_enable_timer_wakeup(INTERVAL_LENGTH * 1000000ULL);
        // esp_deep_sleep_start();

        // If not using deep sleep, uncomment the following line to add some minor delay into the system as needed
        // delay(500); // Delay for 500 milli second before the next loop iteration
    }
}
