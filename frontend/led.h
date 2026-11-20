/**
 * @file led.h
 * @brief RGB status LED control.
 */
#ifndef LED_H
#define LED_H

/**
 * @brief Status LED colors as red (bit 0), green (bit 1) and blue (bit 2) channel masks.
 */
enum led_color {
    LED_OFF     = 0b000, /**< All channels off. */

    LED_RED     = 0b001, /**< Red channel only. */
    LED_GREEN   = 0b010, /**< Green channel only. */
    LED_BLUE    = 0b100, /**< Blue channel only. */

    LED_YELLOW  = 0b011, /**< Red and green channels. */
    LED_MAGENTA = 0b101, /**< Red and blue channels. */
    LED_CYAN    = 0b110, /**< Green and blue channels. */

    LED_WHITE   = 0b111  /**< All channels on. */
};
/**
 * @brief Configure the red, green and blue LED pins as outputs and switch the LED off.
 */
void led_init();
/**
 * @brief Set the RGB status LED color using PWM.
 * @param color Requested status LED color.
 * @note Repeated colors are ignored; green and blue use reduced intensities (GREEN_LEVEL, BLUE_LEVEL).
 */
void led_set(led_color color);

#endif