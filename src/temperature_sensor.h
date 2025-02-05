#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <Arduino.h>

void setupTemperatureSensor();
float readBrewTemperature();
bool isSensorConnected();  // Declare isSensorConnected() to fix the missing function error

#endif
