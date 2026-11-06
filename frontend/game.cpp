#include "game.h"
#include "pinout.h"
#include "temperature.h"
#include "led.h"
#include "phase.h"
#include "epaper.h"
#include "backend.h"

/* PUZZLE CODES */
const char* const puzzle_codes[] = {
    "phase_puzzle_key",
    "serial_puzzle_key",
    "backend_puzzle_key"
};

/* GLOBAL VARIABLES */
game_state state      = PHASE_PUZZLE;
game_state next_state = PHASE_PUZZLE;
game_state prvs_state = PHASE_PUZZLE;

state_codes next_state_code = NO_DOWNLINK_CODE;

static bool phase_solved = false;   

static led_color led_for_state(game_state s) {
    switch (s) {
        case WAIT_INITIALIZATION: return LED_BLUE;
        case PHASE_PUZZLE:        return LED_YELLOW;
        case SERIAL_PUZZLE:       return LED_MAGENTA;
        case BACKEND_PUZZLE:      return LED_CYAN;
        case WAIT_BACKEND:        return LED_WHITE;
        case COMPLETED:           return LED_GREEN;
    }
    return LED_OFF;
}

static bool button_pressed() {
    static bool last_level = HIGH;
    static unsigned long last_change = 0;

    bool level = digitalRead(BUTTON_PIN);
    if (level != last_level && millis() - last_change > 50) {
        last_change = millis();
        last_level  = level;
        return level == LOW;      // true only on press
    }
    return false;
}

void set_state(game_state new_state) {
 
    if (state == new_state)
        return;
 
    prvs_state = state;
    state = new_state;
 
    led_set(led_for_state(state));
    Serial.print(F("STATE "));
    Serial.println(state);
}

/* GAME SETUP */
void game_setup() {

    Serial.begin(57600);
    temperature_init();
    led_init();
    phase_init();
    epaper_init();
    Serial.println(F("Calling backend init"));
    backend_init();

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    led_set(led_for_state(state));   
}

/* GAME STATE MACHINE */
void game_task() {

    epaper_task();   

    switch (state) {

        case WAIT_INITIALIZATION:
            if (button_pressed()) {
                set_state(WAIT_BACKEND);
                break;
            }
            break;

        case PHASE_PUZZLE: {
            if (button_pressed()) {
                set_state(WAIT_BACKEND);
                break;
            }

            bool phase_synced = phase_update();
            bool temp_hot = temperature_triggered();

            if (phase_synced && temp_hot) {
                if (!phase_solved) {
                    Serial.println(F("Phase puzzle solved!"));
                    phase_solved = true;
                    show_code(puzzle_codes[0]);
                }
            } else {
                phase_solved = false;
            }
            break;
        }

        case SERIAL_PUZZLE:
            if (button_pressed())
                set_state(WAIT_BACKEND);
            break;

        case BACKEND_PUZZLE:
            if (button_pressed())
                set_state(WAIT_BACKEND);
            break;

        case COMPLETED:
            // Raise flag
            if (button_pressed())
                set_state(WAIT_BACKEND);
            break;

        case WAIT_BACKEND: {
            next_state_code = get_be_next_state_code();

            switch (next_state_code) {
                case WAIT_INITIALIZATION_CODE: next_state = WAIT_INITIALIZATION; break;
                case PHASE_PUZZLE_CODE:        next_state = PHASE_PUZZLE;        break;
                case SERIAL_PUZZLE_CODE:       next_state = SERIAL_PUZZLE;       break;
                case BACKEND_PUZZLE_CODE:      next_state = BACKEND_PUZZLE;      break;
                case COMPLETED_CODE:           next_state = COMPLETED;           break;
                default: // NO_DOWNLINK / timeout
                    next_state = prvs_state;
                    break;
            }

            if (next_state != prvs_state) {
                phase_solved = false;
                reset_show_code();
            }

            set_state(next_state);
            break;
        }
    }
}