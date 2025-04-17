#ifndef MAIN_H
#define MAIN_H

#include <WiFi.h>
#include <Arduino.h>

void setupTime(); 


// Extern for Telnet client defined in main.cpp
extern WiFiClient telnetClient;

// Generic log without newline
template <typename T>
void conbrew_log(const T &msg)
{
#if ENABLE_SERIAL
    Serial.print(msg);
#endif
    if (telnetClient && telnetClient.connected())
    {
        telnetClient.print(msg);
    }
}

// Generic log with newline
template <typename T>
void conbrew_logln(const T &msg)
{
#if ENABLE_SERIAL
    Serial.println(msg);
#endif
    if (telnetClient && telnetClient.connected())
    {
        telnetClient.println(msg);
    }
}

#endif
