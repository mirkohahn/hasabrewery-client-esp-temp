#ifndef LED_H
#define LED_H

#include <Arduino.h>

void led_on(const String& color);
void led_off(const String& color);
void leds_off();
void led_blink(const String& color, int blink_amount, int ms_on, int ms_off);
void led_blinking(const String& color);
void led_loop(); // must be called from loop() if led_blinking is used

#endif