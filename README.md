# 🚀 HasABrewery ESP32 Client

[![Version](https://img.shields.io/badge/version-1.0.2-brightgreen)]()
[![License](https://img.shields.io/badge/license-Apache%202.0-blue.svg)](LICENSE)  
[![Platform](https://img.shields.io/badge/platform-ESP32-orange)](https://espressif.com/)\
[![Platform](https://img.shields.io/badge/platform-Arduino_IDE-blue?logo=arduino)](https://www.arduino.cc/en/software)
[![Platform](https://img.shields.io/badge/platform-PlatformIO-black?logo=platformio)](https://platformio.org/)




**HasABrewery ESP32 Client** is an IoT-driven temperature monitoring system tailored for ambitious home brewers and small breweries. Built for **ESP32**, it enables real-time temperature logging using **DS18B20 sensors** and seamless integration with **MQTT**. It supports **both static power supply and battery operation**, ensuring uninterrupted monitoring.\
Now able to flash directly thru Arduino IDE or as usually thru Platform IO and VS Code\
Now with integrated deep_sleep you can measure your fermentation's temperature for even longer without worrying about battery levels - too easy, lads! 
- 📊 Click Here [**Web dashboard**](https://github.com/mirkohahn/hasabrewery-host) for real-time monitoring MQTT dashboard.

## 📌 Features

- ✅ **Supporting °F & °C**: Configure using imperial or metric units with a simple click.
- ✅ **Smart WiFi AP Selection**: Automatically connects to the strongest access point.
- ✅ **MQTT-Powered**: Publishes real-time temperature data to a broker.
- ✅ **Precision Monitoring**: Uses **DS18B20 sensors** for accurate readings.
- ✅ **Optimized for PlatformIO**: Easy flashing and configuration.
- ✅ **Supports Static & Battery Power**: Works seamlessly with both power sources.
- ✅ **Multi-sensor support** for multiple DS18B20 probes including ambient temperature.
- ✅ **Battery optimization** to extend operational runtime.

\
Click here to install using the [Arduino IDE](#-installation---arduino-ide) and here for [VS Code & Platform IO](#-installation---vs-code--platform-io)

The current build version for an ESP32 C3 has
````
RAM:   [=         ]  14.1% (used 46296 bytes from 327680 bytes)
Flash: [======    ]  59.9% (used 784701 bytes from 1310720 bytes)
````

## Table of Contents
- [Installation on VS Code & Platform IO](#-installation---vs-code--platform-io) | [Installation using Arduino IDE](#-installation---arduino-ide)
- [Project Structure](#️project-structure)
- [Wiring & Hardware](#configuration)
- [Upcoming Features](#changelog) | [Contribution](#license) | [Support the Project](#license) | [License](#license)
- [Has A Brewery EcoSystem](#license)


## Changelog
| Version | Date       | Description               |
|---------|------------|---------------------------|
| 1.0.3   | 2025-02-27 | Added Arduino files; now flashable thru Arduino IDE            |
| 1.0.2   | 2025-02-11 | Small bug fixes and updates       |
| 1.0.1   | 2025-02-08 | Added Ambient Temperature, Deep Sleep Mode and Imperial Units     |
| 1.0.0   | 2025-02-05 | Working V1 Version;      |



## 📦 (1/2) Installation - VS Code & Platform IO

### **1️⃣ Prerequisites**

- Install **PlatformIO** (recommended) or Arduino IDE
- Install ESP32 toolchain
- Clone the repository:
  ```sh
  git clone https://github.com/mirkohahn/hasabrewery-client-esp-temp.git
  cd hasabrewery-client-esp-temp
  ```

### **2️⃣ Configure WiFi & MQTT**
Easily modify `src/config.h` to include your personal credentials and configurations:
  ```cpp
  #define WIFI_SSID "YourWiFiSSID"
  #define WIFI_PASSWORD "YourWiFiPassword"
  #define MQTT_BROKER "your.mqtt.host-address"
  #define MQTT_PORT 1883
  ```

and much more. You can choose and customize the topic under which the messages are send all from config.h


### **3️⃣ Flash to ESP32**
  ```sh
  pio run --target upload
  ```


## 📦 (2/2) Installation - Arduino IDE

### **1️⃣ Prerequisites**
- Install ESP32 Board Package to Arduino IDE
   -> `File` > `Preferences` 
  then add the following URL to `Additional Board Manager URLs`
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```
- Go to `Tools` > `Board` > `Board Manager`
  Search for **ESP32** and install lates Espressif ESP32 package
- Install required libraries
   go to `Sketch` > `Include Library` > `Manage Libraries` search and install: `PubSubClient` (needed for MQTT messages)

### **2️⃣ Configure your IDE**
- Board:            ESP32C3 Dev Module
- Upload Speed:     115200
- Flash Mode:       QIO
- Port:             (Select the COM port shown for ESP32C3)
- CPU Frequency:    160MHz
- Partition Scheme: Huge APP (3MB No OTA/1MB SPIFFS)


### **3️⃣ Configure WiFi & MQTT**
Easily modify `src/config.h` to include your personal credentials and configurations:
  ```cpp
  #define WIFI_SSID "YourWiFiSSID"
  #define WIFI_PASSWORD "YourWiFiPassword"
  #define MQTT_BROKER "your.mqtt.host-address"
  #define MQTT_PORT 1883
  ```

and much more. You can choose and customize the topic under which the messages are send all from config.h

### **4️⃣ Flash to ESP32**
- select your board from `Tools` > `Board` > `eps32` > Select Your Board 
- click **Upload**

---

## 🛠 Usage
1. **Power on your ESP32 device**.
2. The system **scans and connects to the strongest WiFi AP**.
3. Temperature data is **published via MQTT**.
4. Use an MQTT client like **MQTT Explorer** to monitor the data.

---

## 🖥️ Project Structure
```
📦 hasabrewery-client-esp-temp
 ┣ 📂 include           # Header files
 ┣ 📂 lib               # External libraries (e.g., OneWire)
 ┣ 📂 src               # Source code
 ┃ ┣ 📄 main.cpp        # Main application logic
 ┃ ┣ 📄 config.h        # WiFi & MQTT configuration
 ┃ ┣ 📄 mqtt_handler.cpp # MQTT logic
 ┃ ┗ 📄 temperature_sensor.cpp # Sensor management
 ┣ 📄 platformio.ini    # PlatformIO configuration
 ┣ 📄 .gitignore        # Git ignore rules
 ┣ 📄 README.md         # Project documentation
 ┗ 📄 LICENSE           # License file
```

---

## 🖼️ Wiring & Hardware

### **Pinout Diagrams ESP32 C3 SuperMini and WROOM**
![PinOut Diagram ESP32 C3 SuperMini](additional_assets/pinout_diagram_c3supermini.jpg)  
By default, **GPIO 4** is allocated for **OneWire communication**.

![PinOut Diagram ESP32 WROOM](additional_assets/pinout_diagram.png)  
By default, **GPIO 4** is allocated for **OneWire communication**.


### **Wiring Diagram (ESP32 + Temperature Probe + Battery)**
![Wiring Diagram](additional_assets/WireDiagram_Client_Temp.jpg)
The code uses the OneWire library supporting multiple DS18B20 on the GPIO data pin. However, multiple temperatures (e.g. ambient, beer, etc) are just mocked and not yet implemented.

### **Actual Device Setup**
I am a fan of modular designs and flexibility in my projects. To connect sensors and/or probes or actuators requiring 3 or less wires, I am usually using servo connectors. This allows to easily remove and/or exchange sensors. For this project, I have quite a selection of thermometers (e.g. one without the heatshrink to add to my fermenters thermowell and one in a seperate thermowell for the brewing process) (see picture for reference)

This is a picture of my personal setup. I created two stl files ([bottom]() & [top]()) which are downloadable at printables. They are made for the following components for you to have a battery powered 
![Device Picture](additional_asset/device_picture.png)

---

## 🔮 Upcoming Features
- 🪫 **Battery Level Status** in MQTT Message
- 🚨 **Status LEDs** for easy inspection.
- 📦 **OTA updates** for seamless firmware upgrades.

---

## 📖 Contributing
We welcome contributions! To contribute:
1. **Fork** the repository.
2. Create a **feature branch**:
   ```sh
   git checkout -b feature-new-cool-thing
   ```
3. **Make your changes** and commit:
   ```sh
   git commit -m "Added a new cool feature"
   ```
4. **Push to GitHub and open a Pull Request**!
### Create an New Issue

If you have feedback and/or feature requests, simply create a new issue in the repository

---

## ⭐ Support the Project!
If you find this project useful, please **star the repository ⭐, follow [my GitHub account](https://github.com/mirkohahn)**, and **engage with feedback**. Your support helps us improve and grow the project! 🚀🍻

---

## 📜 License
This project is licensed under the **Apache 2.0 License** – see the [LICENSE](LICENSE) file for details.


---

## 🌐 Has A Brewery Ecosystem
If you like to learn more about **Has A Brewery**, the project and resources, checkout other HasABrewery repositories:
| Name | Description |
|------|------------|
| [Has A Brewery Local Host](https://github.com/mirkohahn/hasabrewery-host) | Cloud backend for data storage and real-time analytics. |
| [Brewery Data Strucutre](https://github.com/mirkohahn/brew_data_structure) | Official website with product details and updates. |
| [Fermentation Chamber Controller](https://github.com/mirkohahn/hasabrewery-fermentation-chamber) | Full Controller for Fermentation & Conditioning Chamber |
| [Keezer / Kegerator Controller](https://github.com/mirkohahn/hasabrewery-keezer-controller) | Coming Soon... |

As well as our website & online tools, such as
| Name | Description |
|------|------------|
| [Get Started Guide](https://hasabrewery.com/get-started) | Full guide, overivew, tutorials and education on how to get started. |
| [Find Ingredients (e.g. Malt & Grinas)](https://hasabrewery.com/tools/ingredients/grain-finder) | Search & Filter over 1,000 grains for EBC, Beer Styles, Taste and more. |
| [Compare Ingredients against eachother](https://hasabrewery.com/tools/ingredients/compare-ingredients) | Benchmark and compare grains (hops, yeasts, etc) against eachother for educated recipe creation |
| [Calculate ABV thru Gravity](https://hasabrewery.com/tools/calculators/abv) | Online Calculator to get Alcohol by Volume from OG and FG gravity reading (SG and °P). |
| [Adjust Gravity Reading for Temp. Difference](https://hasabrewery.com/tools/calculators/hydro-temp-correct) | Calculater the adjustment for your Hydrometer Readings if made at higher/lower temperatures than calibrated. |
| [Look Up Your Local Water Profile](https://hasabrewery.com/tools/charts/water-profiles) | Browse ofer 5,200 water profiles and filter for your region. |

---

## 🎖️ Credits & External Libraries
This project wouldnt be possible without the help of other developers, making tools I utilized. For this this project, I used the **OneWire Library**, which was cloned and adapted from:
- **OneWire Library**: [https://github.com/PaulStoffregen/OneWire](https://github.com/PaulStoffregen/OneWire)

---

 
# 🚀 Happy Brewing & Cheers! 🍻

