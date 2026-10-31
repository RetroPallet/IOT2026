#ifndef GAME_H
#define GAME_H

#include <Arduino.h>

// Enumerate game state.
enum game_state {
    ARMED,
    PHASE_PUZZLE,
    SERIAL_PUZZLE,
    BACKEND_PUZZLE,
    WAIT_BACKEND,
    COMPLETED
};

// Enumerate puzzles.
enum puzzle_id {
    TEMP_PUZZLE_ID,
    PHASE_PUZZLE_ID,
    SERIAL_PUZZLE_ID
};

// "Tell" others that state exists.
extern game_state state;

// Funtction prototypes.
void game_setup();
void game_task();
void set_state(game_state new_state);

#endif