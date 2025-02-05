#include <WiFi.h>
#include <PubSubClient.h>
#include <time.h>  // Include time library
#include "config.h"
#include "mqtt_handler.h"
#include "temperature_sensor.h"  // Ensure this is included

// Global WiFi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// NTP time settings
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;  // Adjust for your timezone
const int   daylightOffset_sec = 0;

// Function to get WiFi Signal Strength (RSSI)
int getWiFiRSSI() {
    return WiFi.RSSI();  // Returns signal strength in dBm
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

// Initialize NTP
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
void publishTemperature(float brewTemp) {
    char topic[128];
    snprintf(topic, sizeof(topic), "receive/%s/%s/%s",
             LOGIC_BREWERY_COMPONENT, DEVICE_TYPE, DEVICE_ID);

    String timestamp = getFormattedTimestamp();
    int wifiRSSI = getWiFiRSSI();  // Get WiFi signal strength

    // Handle sensor errors
    String statusMessage;
    String temperatureStr;

    if (!isSensorConnected()) {
        statusMessage = "ERROR: No DS18B20 detected";
        temperatureStr = "null";
    } else if (brewTemp == -127.00) {
        statusMessage = "ERROR: Sensor read failure";
        temperatureStr = "null";
    } else {
        statusMessage = "OK";
        temperatureStr = String(brewTemp, 2);
    }

    // Construct JSON payload with expanded status information
    char payload[512];  // Increased buffer size for larger JSON
    snprintf(payload, sizeof(payload),
             "{\"timestamp\": \"%s\", \"temp_ambient\": null, \"temp_brew\": %s, \"status\": {\"status_message\": \"%s\", \"transmission_type\": \"%s\", \"RSSI\": %d}}",
             timestamp.c_str(), temperatureStr.c_str(), statusMessage.c_str(), TRANSMISSION_TYPE, wifiRSSI);

    if (client.publish(topic, payload)) {
        Serial.print("📡 Published to topic ");
        Serial.print(topic);
        Serial.print(": ");
        Serial.println(payload);
    } else {
        Serial.println("❌ Failed to publish MQTT message");
    }
}
