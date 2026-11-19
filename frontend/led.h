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
    LED_OFF     = 0b000,

    LED_RED     = 0b001,
    LED_GREEN   = 0b010,
    LED_BLUE    = 0b100,

    LED_YELLOW  = 0b011,
    LED_MAGENTA = 0b101,
    LED_CYAN    = 0b110,

    LED_WHITE   = 0b111
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