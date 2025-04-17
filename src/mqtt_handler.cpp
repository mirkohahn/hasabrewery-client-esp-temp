#include <WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include "config.h"
#include "mqtt_handler.h"
#include "temperature_sensor.h"
#include "led.h"
#include "battery_monitor.h"
#include "main.h"

// Global WiFi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// NTP time settings
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;     // ADJUST in SECONDS! for our timezone -> e.g. +1h for CET = 3600
const int daylightOffset_sec = 0; // ADJUST in SECONDS! for our daylight saving time

// Function to get WiFi Signal Strength (RSSI)
int getWiFiRSSI()
{
    return WiFi.RSSI(); // Returns signal strength in dBm
    // -30 dBm: Amazing, -67 dBm: Very Good, -70 dBm: Okay, -80 dBm: Not Good, -90 dBm: Unusable
}

// Function to get formatted timestamp
String getFormattedTimestamp()
{
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        conbrew_logln("⚠️ Failed to obtain time. Using default 1970 timestamp.");
        return "1970-01-01 00:00:00";
    }

    char formattedTime[30];
    strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(formattedTime);
}

// Initialize NTP - Network Time Protocoll
void setupTime()
{
    conbrew_logln("⏳ Syncing time via NTP...");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(2000);
    conbrew_logln("✅ Time synchronized!");
}

// Function to handle MQTT reconnection
void reconnect()
{
    while (!client.connected())
    {
        conbrew_logln("🔄 Attempting MQTT connection...");
        led_blink("blue", 5, 200, 200);
        if (client.connect(DEVICE_ID))
        {
            conbrew_logln("✅ Connected to MQTT broker.");
            led_off("red");
            led_on("blue");
        }
        else
        {
            conbrew_log("❌ Connection failed, rc=");
            led_on("red");
            led_blink("blue", 5, 250, 250);
            Serial.print(client.state());
            conbrew_logln(" retrying in 5 seconds...");
            delay(5000);
        }
    }
}

// Initialize MQTT
void mqttInit()
{
    client.setServer(MQTT_BROKER, MQTT_PORT);
}

// Maintain MQTT connection
void mqttLoop()
{
    if (!client.connected())
    {
        led_blink("red", 2, 200, 200);
        led_blink("blue", 2, 200, 200);
        reconnect();
    }
    client.loop();
}

// Publish temperature data to MQTT with enhanced status
void publishTemperature(float brewTemp, float ambientTemp)
{
    conbrew_logln("🛠 DEBUG: Inside publishTemperature()");

    Serial.print("🚀 Sending Brew Temp: ");
    conbrew_logln(brewTemp);

    Serial.print("🚀 Sending Ambient Temp: ");
    conbrew_logln(ambientTemp);

    float batteryLevel = get_battery_status();

    char topic[128];
    snprintf(topic, sizeof(topic), "receive/%s/%s/%s",
             LOGIC_BREWERY_COMPONENT, DEVICE_TYPE, DEVICE_ID);

    String timestamp = getFormattedTimestamp();
    int wifiRSSI = getWiFiRSSI();

    // Construct JSON payload with "values" array
    char payload[512];
    snprintf(payload, sizeof(payload),
             "{\"timestamp\": \"%s\", \"values\": [{\"temp_ambient\": %s}, {\"temp_brew\": %s}], \"status\": {\"status_message\": \"%s\", \"battery_level\": %s, \"transmission_type\": \"%s\", \"RSSI\": %d}}",
             timestamp.c_str(),
             (ambientTemp == -127.00) ? "null" : String(ambientTemp, 2).c_str(),
             (brewTemp == -127.00) ? "null" : String(brewTemp, 2).c_str(),
             "OK", (batteryLevel < 0) ? "null" : String(batteryLevel, 1).c_str(),
             TRANSMISSION_TYPE, wifiRSSI);

    conbrew_log("📡 MQTT Payload: ");
    conbrew_logln(payload);

    if (client.publish(topic, payload))
    {
        conbrew_logln("✅ MQTT Publish Success!");
        led_off("blue");
        led_blink("blue", 5, 100, 100);
        led_on("blue");
    }
    else
    {
        conbrew_logln("❌ Failed to publish MQTT message.");
        led_blink("red", 5, 100, 100);
        led_blink("blue", 5, 100, 100);
    }
}
