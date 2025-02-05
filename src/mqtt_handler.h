#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <Arduino.h>

void mqttInit();
void mqttLoop();
void publishTemperature(float brewTemp);
void reconnect();  // Declare reconnect() to avoid errors

#endif
