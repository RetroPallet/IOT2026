#include "game.h"
#include "temperature.h"
#include "led.h"
#include "phase.h"

/* DEBUG things */
/* This is neccessary cz testing code with faulty dht11.*/
unsigned long debug_started_at = 0; // REMOVE in production.
const unsigned long DEBUG_INTERVAL = 4000; // REMOVE in production.

#define BUTTON_PIN 2

/* PUZZLE CODES */
const char* const puzzle_codes[] = {
    "X7K9",
    "P4M2",
    "R8T3"
};

/* GLOBAL VARIABLES */
game_state state      = ARMED;
game_state next_state = ARMED;
game_state prvs_state = ARMED;


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
    led_set(LED_YELLOW);
    phase_init();

    pinMode(BUTTON_PIN, INPUT_PULLUP); // REMOVE in production.
    debug_started_at = millis(); // REMOVE in production.
}

/* GAME STATE MACHINE */
void game_task() {

    switch (state) {

        case ARMED:
            // Temperature puzzle.
            if (temperature_triggered() ||
                (millis() - debug_started_at >= DEBUG_INTERVAL)) { // REMOVE in production.

                next_state = PHASE_PUZZLE;
                led_set(LED_RED);
                set_state(WAIT_BACKEND);
            }
            break;

        case PHASE_PUZZLE:
            // LED synchronization puzzle.
            if (phase_update()) {
                next_state = SERIAL_PUZZLE;
                set_state(WAIT_BACKEND);
            }
            break;

        case SERIAL_PUZZLE:
            // Serial password challenge.
            break;

        case BACKEND_PUZZLE:
            // Backend SQL injection challenge.
            break;

        case WAIT_BACKEND:

            switch (prvs_state) {

                case ARMED:
                    if (digitalRead(BUTTON_PIN) == LOW) { // REMOVE in production.
                        led_set(LED_GREEN);
                        set_state(next_state);
                    }
                    break;

                case PHASE_PUZZLE:
                    if (digitalRead(BUTTON_PIN) == LOW) { // REMOVE in production.
                        led_set(LED_RED);
                        set_state(next_state);
                    }                    
                    break;

                case SERIAL_PUZZLE: 
                    if (digitalRead(BUTTON_PIN) == LOW) { // REMOVE in production.
                        set_state(next_state);
                    }      
                    break;

                default:
                    break;
            }
            break;

        case COMPLETED:
            // Raise flag
            break;
    }
}