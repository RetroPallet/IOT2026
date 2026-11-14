/**
 * @file led.h
 * @brief Two-channel status LED control.
 */
#ifndef LED_H
#define LED_H

/**
 * @brief Red and green channel bit masks for the status LED.
 */
enum led_color {
    LED_OFF    = 0b00, /**< Both LED channels off. */
    LED_RED    = 0b01, /**< Red channel enabled. */
    LED_GREEN  = 0b10, /**< Green channel enabled. */
    LED_YELLOW = 0b11 /**< Both channels enabled with yellow mixing. */
};

/**
 * @brief Configure the red and green LED pins as outputs and switch the LED off.
 */
void led_init();
/**
 * @brief Set the two-channel status LED color using PWM.
 * @param color Requested status LED color.
 * @note Repeated colors are ignored; yellow uses a reduced green intensity.
 */
void led_set(led_color color);

#endif