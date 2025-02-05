#ifndef CONFIG_H
#define CONFIG_H

// WiFi credentials
// #define WIFI_SSID       "yourSSID"
// #define WIFI_PASSWORD   "yourPASSWORD"
#define WIFI_SSID       "VM7498479"
#define WIFI_PASSWORD   "skdAf2pN6vpyygas"

// Device and component configuration
#define LOGIC_BREWERY_COMPONENT  "fermenter"
#define DEVICE_TYPE              "temperature_sensor"
// #define DEVICE_ID                "A1B2C3"
#define DEVICE_ID                "1A2B3C"

#define TRANSMISSION_TYPE        "wifi"     // TODO: Add support for BT (and Zigbee?)

// Transmission interval in seconds
#define INTERVAL_LENGTH          1         // Interval between transmissions in sec

// MQTT Broker configuration
// #define MQTT_BROKER              "your.host.ip"  // Replace with your broker IP/hostname
#define MQTT_BROKER              "192.168.0.209" 
#define MQTT_PORT                1883           // Default MQTT port

#endif
