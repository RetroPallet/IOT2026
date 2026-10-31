#include <Arduino.h>
#include "led.h"

#define LED_RED_PIN   5
#define LED_GREEN_PIN 6

void led_init() {
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
}

void led_set(led_color color) {
  analogWrite(LED_RED_PIN, (color & LED_RED) ? 255 : 0); // if red bit set.
  analogWrite(LED_GREEN_PIN, (color & LED_GREEN) ? 80 : 0); // if green bit set, correct for unbalance with 70 pwm.
}