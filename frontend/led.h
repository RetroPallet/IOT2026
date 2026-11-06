#ifndef LED_H
#define LED_H

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

void led_init();
void led_set(led_color color);

#endif