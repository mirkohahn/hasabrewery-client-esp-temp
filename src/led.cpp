#include "led.h"
#include "config.h"

#if ENABLE_LED

namespace {
  unsigned long previousMillis = 0;
  bool ledState = false;
  String blinkingColor = "";
}

int getPinForColor(const String& color) {
  if (color == "red") return PIN_LED_RED;
  if (color == "green") return PIN_LED_GREEN;
  if (color == "blue") return PIN_LED_BLUE;
  return -1;
}

void led_on(const String& color) {
  int pin = getPinForColor(color);
  if (pin != -1) {
    blinkingColor = ""; // stop infinite blinking
    digitalWrite(pin, HIGH);
  }
}

void led_off(const String& color) {
  int pin = getPinForColor(color);
  if (pin != -1) {
    blinkingColor = ""; // stop infinite blinking
    digitalWrite(pin, LOW);
  }
}

void leds_off() {
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_BLUE, LOW);
}

void led_blink(const String& color, int blink_amount, int ms_on, int ms_off) {
  int pin = getPinForColor(color);
  if (pin == -1) return;

  for (int i = 0; i < blink_amount; i++) {
    digitalWrite(pin, HIGH);
    delay(ms_on);
    digitalWrite(pin, LOW);
    delay(ms_off);
  }
}

void led_blinking(const String& color) {
  blinkingColor = color;
  previousMillis = millis();
}

void led_loop() {
  if (blinkingColor == "") return;

  int pin = getPinForColor(blinkingColor);
  if (pin == -1) return;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 500) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(pin, ledState ? HIGH : LOW);
  }
}

#endif
