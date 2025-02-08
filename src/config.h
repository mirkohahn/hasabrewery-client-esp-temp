#ifndef CONFIG_H
#define CONFIG_H

// Wireless Transmission Type
#define FIRMWARE_VERSION         "1.0.1"   // Firmware version
#define TRANSMISSION_TYPE        "wifi"     // TODO: Add support for BT (and Zigbee?)
// Transmission Interval in Seconds
#define INTERVAL_LENGTH          30         // Interval between transmissions in sec

// TO DO FOR USERS
// --> in mqtt_handler.cpp -> update the time stamp correction for your timezone +/- GMT

// IMPERIAL NOT YET WORKING
#define IMPERIAL_UNITS false                 // Set to true for °F, false for °C

// <--- W I F I   C O N F I G U R A T I O N --->
#define WIFI_SSID       "yourSSID"          // Replace with your SSID
#define WIFI_PASSWORD   "yourPASSWORD"      // Replace with your WiFi password



// <--- M Q T T   S E T U P   &   S T R U C T U R E --->
#define MQTT_BROKER              "your.host.ip"     // Replace with your broker IP/hostname
#define MQTT_PORT                1883               // Default MQTT port; change if needed

#define LOGIC_BREWERY_COMPONENT  "fermenter"
#define DEVICE_TYPE              "temperature_sensor"
#define DEVICE_ID                "1A2B3C"           // Replace with your device ID; Reocmmended: Random 6-digit hex


#endif
