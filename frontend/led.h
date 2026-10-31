#ifndef LED_H
#define LED_H


/* Enumerate only the colors we need.*/
enum led_color {
    LED_RED    = 0b01,
    LED_GREEN  = 0b10,
    LED_YELLOW = 0b11
};

void led_init();
void led_set(led_color color);

#endif