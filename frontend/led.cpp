/**
 * @file led.cpp
 * @brief Two-channel status LED control.
 */
#include <Arduino.h>
#include "led.h"
#include "pinout.h"

/**
 * @brief Green PWM intensity.
 */
#define GREEN_LEVEL 80
/**
 * @brief Blue PWM intensity.
 */
#define BLUE_LEVEL  80

static uint8_t current = 0xFF;

/**
 * @brief Configure the red and green LED pins as outputs and switch the LED off.
 */
void led_init() {
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);
    led_set(LED_OFF);
}

/**
 * @brief Set the two-channel status LED color using PWM.
 * @param color Requested status LED color.
 * @note Repeated colors are ignored; yellow uses a reduced green intensity.
 */
void led_set(led_color color) {
    if (color == current) return;
    current = color;

    analogWrite(LED_RED_PIN,   (color & LED_RED)   ? 255         : 0);
    analogWrite(LED_GREEN_PIN, (color & LED_GREEN) ? GREEN_LEVEL : 0);
    analogWrite(LED_BLUE_PIN,  (color & LED_BLUE)  ? BLUE_LEVEL  : 0);
}