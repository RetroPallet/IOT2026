#include <Arduino.h>
#include "led.h"
#include "pinout.h"

#define GREEN_LEVEL 80
#define BLUE_LEVEL  80

static uint8_t current = 0xFF;

void led_init() {
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);
    led_set(LED_OFF);
}

void led_set(led_color color) {
    if (color == current) return;
    current = color;

    analogWrite(LED_RED_PIN,   (color & LED_RED)   ? 255         : 0);
    analogWrite(LED_GREEN_PIN, (color & LED_GREEN) ? GREEN_LEVEL : 0);
    analogWrite(LED_BLUE_PIN,  (color & LED_BLUE)  ? BLUE_LEVEL  : 0);
}
