#ifndef LED_H
#define LED_H

enum led_color {
    LED_OFF    = 0b00,
    LED_RED    = 0b01,
    LED_GREEN  = 0b10,
    LED_YELLOW = 0b11
};

void led_init();
void led_set(led_color color);

#endif