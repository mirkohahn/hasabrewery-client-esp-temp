#ifndef CONFIG_H
#define CONFIG_H

// Wireless Transmission Type
#define FIRMWARE_VERSION         "1.1.0"     // Firmware version
#define TRANSMISSION_TYPE        "wifi"     // TODO: Add support for BLE (and Zigbee?)
// Transmission Interval in Seconds
#define INTERVAL_LENGTH          10         // Interval between transmissions in sec


// IMPERIAL NOT YET WORKING
#define IMPERIAL_UNITS false                 // Set to true for °F, false for °C


// <--- W I F I   C O N F I G U R A T I O N --->
#define WIFI_SSID       "VM2214596"             // Replace with your SSID
#define WIFI_PASSWORD   "iow9wQsey6Yodfdy"      // Replace with your WiFi password



// <--- M Q T T   S E T U P   &   S T R U C T U R E --->
#define MQTT_BROKER              "192.168.0.57"     // Replace with your broker IP/hostname
#define MQTT_PORT                1883               // Default MQTT port; change if needed

#define LOGIC_BREWERY_COMPONENT  "mash_tun"
#define DEVICE_TYPE              "thermometer"
#define DEVICE_ID                "6A38F5"           // Replace wit h your device ID; Reocmmended: Random 6-digit hex


// ================================================================= 
//          EVERYTHING BELOW THIS LINE IS OPTIONAL
// ================================================================= 

// <--- F E A T U R E   T O G G L E S --->
#define ENABLE_LED              true    // set to false to disable features
#define ENABLE_BATTERY_MONITOR  true
#define ENABLE_CAN_MODULE       true



// <--- G P I O   P I N    A L L O C A T I O N --->
#define PIN_LED_RED            21
#define PIN_LED_GREEN          10
#define PIN_LED_BLUE           20

#define PIN_BATTERY_SDA        8
#define PIN_BATTERY_SCL        9

#define PIN_CAN_RX             1
#define PIN_CAN_TX             0




#endif
