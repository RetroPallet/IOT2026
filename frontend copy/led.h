#ifndef LED_H
#define LED_H


/* COLORS */
enum led_color {
    LED_OFF     = 0b000, 

    LED_RED     = 0b001, // ARMED
    LED_GREEN   = 0b010, // COMPLETED
    LED_BLUE    = 0b100, // INITIAL

    LED_YELLOW  = 0b011, // PHASE_PUZZLE
    LED_MAGENTA = 0b101, // SERIAL_PUZZLE
    LED_CYAN    = 0b110, //BACKEND_PUZZLE

    LED_WHITE   = 0b111  // WAIT_BACKEND
};
void led_init();
void led_set(led_color color);

#endif