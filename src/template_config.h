// this is only a TEMPLATE FILE!
// Copy/Rename this file to config.h and edit the values to match your local setup

#ifndef CONFIG_H
#define CONFIG_H

// Wireless Transmission Type
#define FIRMWARE_VERSION         "1.1.1"    
#define TRANSMISSION_TYPE        "wifi"     
// Transmission Interval in Seconds
#define INTERVAL_LENGTH          10         // Interval between transmissions in sec


// IMPERIAL NOT YET WORKING
#define IMPERIAL_UNITS false                 // Set to true for °F, false for °C


// <--- W I F I   C O N F I G U R A T I O N --->
#define WIFI_SSID       "your-wifi-name"             // Replace with your SSID
#define WIFI_PASSWORD   "your-wifi-password"      // Replace with your WiFi password


// <--- D E E P   S L E E P   C O N F I G U R A T I O N --->
#define SLEEP_TIMER       2500            // sleep for x ms; set to 0 to disable deep sleep


// <--- U P D A T E   &   M O N I T O R I N G   C O N F I G U R A T I O N --->
#define TELNET_PORT              23              // Telnet port for OTA updates
#define INITIAL_DEV_TIME         120000         // Initial time to wait for OTA updates and host Telnet Server in ms


// <--- M Q T T   S E T U P   &   S T R U C T U R E --->
#define MQTT_BROKER              "192.168.0.xxx"     // Replace with your broker IP/hostname
#define MQTT_PORT                1883               // Default MQTT port; change if needed

#define LOGIC_BREWERY_COMPONENT  "all_in_one_unit"
#define DEVICE_TYPE              "thermometer"
#define DEVICE_ID                "A1B2C3"           // Replace wit h your device ID; Reocmmended: Random 6-digit hex


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
