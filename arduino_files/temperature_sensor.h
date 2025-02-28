#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H



void setupTemperatureSensor();
float readBrewTemperature();
float readAmbientTemperature();
bool isSensorConnected();

#endif
