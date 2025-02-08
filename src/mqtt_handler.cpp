#include <WiFi.h>
#include <PubSubClient.h>
#include <time.h>  
#include "config.h"
#include "mqtt_handler.h"
#include "temperature_sensor.h"

// Global WiFi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// NTP time settings
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;              // ADJUST in SECONDS! for our timezone -> e.g. +1h for CET = 3600
const int   daylightOffset_sec = 0;         // ADJUST in SECONDS! for our daylight saving time

// Function to get WiFi Signal Strength (RSSI)
int getWiFiRSSI() {
    return WiFi.RSSI();  // Returns signal strength in dBm
    // -30 dBm: Amazing, -67 dBm: Very Good, -70 dBm: Okay, -80 dBm: Not Good, -90 dBm: Unusable
}

// Function to get formatted timestamp
String getFormattedTimestamp() {
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("⚠️ Failed to obtain time. Using default 1970 timestamp.");
        return "1970-01-01 00:00:00";
    }

    char formattedTime[30];
    strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(formattedTime);
}

// Initialize NTP - Network Time Protocoll
void setupTime() {
    Serial.println("⏳ Syncing time via NTP...");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(2000);
    Serial.println("✅ Time synchronized!");
}

// Function to handle MQTT reconnection
void reconnect() {
    while (!client.connected()) {
        Serial.print("🔄 Attempting MQTT connection...");
        if (client.connect(DEVICE_ID)) {
            Serial.println("✅ Connected to MQTT broker.");
        } else {
            Serial.print("❌ Connection failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds...");
            delay(5000);
        }
    }
}

// Initialize MQTT
void mqttInit() {
    client.setServer(MQTT_BROKER, MQTT_PORT);
}

// Maintain MQTT connection
void mqttLoop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}

// Publish temperature data to MQTT with enhanced status
void publishTemperature(float brewTemp, float ambientTemp) {
    Serial.println("🛠 DEBUG: Inside publishTemperature()");
    
    Serial.print("🚀 Sending Brew Temp: ");
    Serial.println(brewTemp);

    Serial.print("🚀 Sending Ambient Temp: ");
    Serial.println(ambientTemp);

    char topic[128];
    snprintf(topic, sizeof(topic), "receive/%s/%s/%s",
             LOGIC_BREWERY_COMPONENT, DEVICE_TYPE, DEVICE_ID);

    String timestamp = getFormattedTimestamp();
    int wifiRSSI = getWiFiRSSI();  

    // Construct JSON payload
    char payload[512];  
    snprintf(payload, sizeof(payload),
             "{\"timestamp\": \"%s\", \"temp_ambient\": %s, \"temp_brew\": %s, \"status\": {\"status_message\": \"%s\", \"transmission_type\": \"%s\", \"RSSI\": %d}}",
             timestamp.c_str(), (ambientTemp == -127.00) ? "null" : String(ambientTemp, 2).c_str(),
             (brewTemp == -127.00) ? "null" : String(brewTemp, 2).c_str(),
             "OK", TRANSMISSION_TYPE, wifiRSSI);

    Serial.print("📡 MQTT Payload: ");
    Serial.println(payload);

    if (client.publish(topic, payload)) {
        Serial.println("✅ MQTT Publish Success!");
    } else {
        Serial.println("❌ Failed to publish MQTT message.");
    }
}



