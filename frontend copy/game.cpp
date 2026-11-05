#include "game.h"
#include "temperature.h"
#include "led.h"
#include "phase.h"
#include "epaper.h"
#include "backend.h"

#define BUTTON_PIN 2

/* PUZZLE CODES */
const char* const puzzle_codes[] = {
    "phase_puzzle_key",
    "serial_puzzle_key",
    "backend_puzzle_key"
};

/* GLOBAL VARIABLES */
game_state state      = WAIT_INITIALIZATION;
game_state next_state = WAIT_INITIALIZATION;
game_state prvs_state = WAIT_INITIALIZATION;

state_codes next_state_code = NO_DOWNLINK_CODE;

led_color prev_led_color = LED_BLUE;
/* STATE MANAGEMENT */
void set_state(game_state new_state) {

    if (state == new_state)
        return;

    prvs_state = state;
    state = new_state;
}

/* GAME SETUP */
void game_setup() {
    temperature_init();
    led_init();
    led_set(LED_BLUE);
    phase_init();

    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

/* GAME STATE MACHINE */
void game_task() {

    switch (state) {

        case ARMED:
            prev_led_color = LED_RED;
            if (digitalRead(BUTTON_PIN) == LOW){
                led_set(LED_WHITE);
                set_state(WAIT_BACKEND);
            }
            // Temperature puzzle.
            else if (temperature_triggered() && phase_update()) {

                led_set(LED_YELLOW);
                set_state(PHASE_PUZZLE);
            }
            break;

        case PHASE_PUZZLE:
            prev_led_color = LED_YELLOW;
            if (digitalRead(BUTTON_PIN) == LOW){
                led_set(LED_WHITE);
                set_state(WAIT_BACKEND);
            }
            // LED synchronization puzzle.
            else if (!phase_update()) {
                led_set(LED_RED);
                set_state(ARMED);
            }
            else{
                show_code(puzzle_codes[0]);
            }
            break;

        case SERIAL_PUZZLE:
            prev_led_color = LED_MAGENTA;
            if (digitalRead(BUTTON_PIN) == LOW){
                led_set(LED_WHITE);
                set_state(WAIT_BACKEND);
            }            
            break;

        case BACKEND_PUZZLE:
            prev_led_color = LED_CYAN;
            if (digitalRead(BUTTON_PIN) == LOW){
                led_set(LED_WHITE);
                set_state(WAIT_BACKEND);
            }                     
            break;

        case COMPLETED:
            prev_led_color = LED_GREEN;
            // Raise flag
            break;

        case WAIT_BACKEND:
            next_state_code = get_be_next_state_code();
            switch (next_state_code) {
                case WAIT_INITIALIZATION_CODE:
                    led_set(LED_BLUE);
                    next_state = WAIT_INITIALIZATION;
                    break;
                case ARMED_CODE:
                    led_set(LED_RED);
                    next_state = ARMED;
                    break;
                case SERIAL_PUZZLE_CODE:
                    led_set(LED_MAGENTA);
                    next_state = SERIAL_PUZZLE;
                    break;
                case BACKEND_PUZZLE_CODE:
                    led_set(LED_CYAN);
                    next_state = BACKEND_PUZZLE;
                    break;
                case COMPLETED_CODE:
                    led_set(LED_GREEN);
                    next_state = COMPLETED;
                    break;
                default: //case no downlink or timeout
                    next_state = prvs_state;
                    led_set(prev_led_color);
            }

    }
}