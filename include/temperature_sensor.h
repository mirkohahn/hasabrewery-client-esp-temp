#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <Arduino.h>

void setupTemperatureSensor();
float readBrewTemperature();
bool isSensorConnected();  

#endif
