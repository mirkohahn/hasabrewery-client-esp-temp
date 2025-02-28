#ifndef CONFIG_H
#define CONFIG_H

// Wireless Transmission Type
#define FIRMWARE_VERSION         "1.0.3"   // Firmware version
#define TRANSMISSION_TYPE        "wifi"     // TODO: Add support for BT (and Zigbee?)
// Transmission Interval in Seconds
#define INTERVAL_LENGTH          10         // Interval between transmissions in sec

// TO DO FOR USERS
// --> in mqtt_handler.cpp -> update the time stamp correction for your timezone +/- GMT

// IMPERIAL NOT YET WORKING
#define IMPERIAL_UNITS false                 // Set to true for °F, false for °C

// <--- W I F I   C O N F I G U R A T I O N --->
#define WIFI_SSID       "VM7498479"          // Replace with your SSID
#define WIFI_PASSWORD   "skdAf2pN6vpyygas"      // Replace with your WiFi password



// <--- M Q T T   S E T U P   &   S T R U C T U R E --->
#define MQTT_BROKER              "192.168.0.56"     // Replace with your broker IP/hostname
#define MQTT_PORT                1883               // Default MQTT port; change if needed

#define LOGIC_BREWERY_COMPONENT  "arduino"
#define DEVICE_TYPE              "thermometer"
#define DEVICE_ID                "132456"           // Replace wit h your device ID; Reocmmended: Random 6-digit hex


#endif
