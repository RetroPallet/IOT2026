#include <Arduino.h>
#include "led.h"
#include "pinout.h"

#define GREEN_IN_YELLOW 40

static uint8_t current = 0xFF;

void led_init() {
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    led_set(LED_OFF);
}

void led_set(led_color color) {
    if (color == current) return;
    current = color;

    uint8_t green = (color == LED_YELLOW) ? GREEN_IN_YELLOW : 255;
    analogWrite(LED_RED_PIN,   (color & LED_RED)   ? 255   : 0);
    analogWrite(LED_GREEN_PIN, (color & LED_GREEN) ? green : 0);
}